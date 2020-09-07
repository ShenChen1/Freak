#define OS_LINUX
#include "librtsp/rtsp-client.h"
#include "librtsp/rtsp-header-transport.h"
#include "cstringext.h"
#include "ntp-time.h"
#include "rtp-media.h"
#include "rtp-transport.h"
#include "rtp-receiver.h"
#include "sockpair.h"
#include "sockutil.h"
#include "uri-parse.h"
#include "urlcodec.h"

typedef struct {
    socket_t socket;

    int transport;
    socket_t rtp[5][2];
    uint16_t port[5][2];

    struct rtsp_client_t* rtsp;
} rtsp_client_priv_t;

static int rtsp_client_send(void* param,
                            const char* uri,
                            const void* req,
                            size_t bytes)
{
    // TODO: check uri and make socket
    // 1. uri != rtsp describe uri(user input)
    // 2. multi-uri if media_count > 1
    rtsp_client_priv_t* priv = param;
    return socket_send_all_by_time(priv->socket, req, bytes, 0, 2000);
}

static int rtpport(void* param,
                   int media,
                   const char* source,
                   unsigned short port[2],
                   char* ip,
                   int len)
{
    rtsp_client_priv_t* priv = param;

    switch (priv->transport) {
    case RTSP_TRANSPORT_RTP_UDP:
        sockpair_create(NULL, priv->rtp[media], priv->port[media]);
        port[0] = priv->port[media][0];
        port[1] = priv->port[media][1];
        break;

    case RTSP_TRANSPORT_RTP_TCP:
        port[0] = 2 * media;
        port[1] = 2 * media + 1;
        break;

    default:
        assert(0);
        return -1;
    }

    return 0;
}

static int ondescribe(void* param, const char* sdp)
{
    rtsp_client_priv_t* priv = param;
    return rtsp_client_setup(priv->rtsp, sdp);
}

static int onsetup(void* param, int64_t duration)
{
    int i;
    uint64_t npt = 0;
    rtsp_client_priv_t* priv = param;
    rtsp_client_play(priv->rtsp, &npt, NULL);

    for (i = 0; i < rtsp_client_media_count(priv->rtsp); i++) {
        int payload, port[2];
        const char* encoding;
        const struct rtsp_header_transport_t* transport;
        transport = rtsp_client_get_media_transport(priv->rtsp, i);
        encoding = rtsp_client_get_media_encoding(priv->rtsp, i);
        payload = rtsp_client_get_media_payload(priv->rtsp, i);

        if (RTSP_TRANSPORT_RTP_UDP == transport->transport) {
            // only
            assert(0 == transport->multicast);  // unicast only
            assert(transport->rtp.u.client_port1 == priv->port[i][0]);
            assert(transport->rtp.u.client_port2 == priv->port[i][1]);

            port[0] = transport->rtp.u.server_port1;
            port[1] = transport->rtp.u.server_port2;
            if (*transport->source) {
                rtp_udp_receiver_create(priv->rtp[i], transport->source, port, payload, encoding);
            } else {
                char ip[65];
	            uint16_t rtspport;
                socket_getpeername(priv->socket, ip, &rtspport);
                rtp_udp_receiver_create(priv->rtp[i], ip, port, payload, encoding);
            }
        } else if (RTSP_TRANSPORT_RTP_TCP == transport->transport) {
            rtp_tcp_receiver_create(transport->interleaved1, transport->interleaved2, payload, encoding);
        } else {
            assert(0);  // TODO
        }
    }

    return 0;
}

static int onplay(void* param,
                  int media,
                  const uint64_t* nptbegin,
                  const uint64_t* nptend,
                  const double* scale,
                  const struct rtsp_rtp_info_t* rtpinfo,
                  int count)
{
    return 0;
}

static int onpause(void* param)
{
    return 0;
}

static int onteardown(void* param)
{
    rtsp_client_priv_t* priv = param;
    return rtsp_client_teardown(priv->rtsp);
}

static void onrtp(void* param,
                  uint8_t channel,
                  const void* data,
                  uint16_t bytes)
{
}

void* rtsp_client_init(const char* url)
{
    rtsp_client_priv_t* priv = NULL;

    priv = malloc(sizeof(rtsp_client_priv_t));
    if (priv == NULL) {
        return NULL;
    }

    struct rtsp_client_handler_t handler = {};
    handler.send = rtsp_client_send;
    handler.rtpport = rtpport;
    handler.ondescribe = ondescribe;
    handler.onsetup = onsetup;
    handler.onplay = onplay;
    handler.onpause = onpause;
    handler.onteardown = onteardown;
    handler.onrtp = onrtp;

    memset(priv, 0, sizeof(rtsp_client_priv_t));
    priv->transport = RTSP_TRANSPORT_RTP_UDP;
    priv->socket = socket_connect_host(url, 1234, -1);
    if (priv->socket == socket_invalid) {
        goto err;
    }
    socket_setnonblock(priv->socket, 1);
    priv->rtsp = rtsp_client_create(url, NULL, NULL, &handler, priv);
    rtsp_client_describe(priv->rtsp);

    return priv;

err:
    free(priv);
    return NULL;
}

int rtsp_client_uninit(void* rtsp)
{
    rtsp_client_priv_t* priv = rtsp;

    rtsp_client_destroy(priv->rtsp);
    socket_close(priv->socket);
    free(priv);

    return 0;
}
