#include "log.h"
#include "sys/thread.h"
#include "librtsp/rtsp-client.h"
#include "librtsp/rtsp-header-rtp-info.h"
#include "librtsp/rtsp-header-transport.h"
#include "cstringext.h"
#include "ntp-time.h"
#include "rtp-media.h"
#include "rtp-transport.h"
#include "rtp-receiver.h"
#include "sockpair.h"
#include "sockutil.h"
#include "uri-parse.h"

typedef struct {
    socket_t socket;
    pthread_t thread;

    int transport;
    socket_t rtp[5][2];
    uint16_t port[5][2];

    struct rtsp_client_t* rtsp;
    struct rtp_receiver_t* receiver[5];

    char packet[2 * 1024 * 1024];
} rtsp_client_priv_t;

static int rtsp_client_recv(void *param)
{
    int ret;
    rtsp_client_priv_t* priv = param;

    socket_setnonblock(priv->socket, 0);
    while (1) {
        ret = socket_recv(priv->socket, priv->packet, sizeof(priv->packet), 0);
        if (ret <= 0) {
            break;
        }
        tracef("rtsp_client_input:%s", priv->packet);
        rtsp_client_input(priv->rtsp, priv->packet, ret);
    };

    return 0;
}

static int rtsp_client_send(void* param,
                            const char* uri,
                            const void* req,
                            size_t bytes)
{
    // TODO: check uri and make socket
    // 1. uri != rtsp describe uri(user input)
    // 2. multi-uri if media_count > 1
    rtsp_client_priv_t* priv = param;
    tracef("param:%p uri:%s req:%p bytes:%zu", param, uri, req, bytes);

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
    tracef("param:%p media:%d source:%s port:%p ip:%p len:%d",
        param, media, source, port, ip, len);

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

    return priv->transport;
}

static int ondescribe(void* param, const char* sdp)
{
    rtsp_client_priv_t* priv = param;
    tracef("param:%p sdp:%s", param, sdp);

    return rtsp_client_setup(priv->rtsp, sdp);
}

static int onsetup(void* param, int64_t duration)
{
    int i;
    uint64_t npt = 0;
    rtsp_client_priv_t* priv = param;
    tracef("param:%p duration:%ld", param, duration);

    rtsp_client_play(priv->rtsp, &npt, NULL);
    for (i = 0; i < rtsp_client_media_count(priv->rtsp); i++) {
        int payload;
        const char* encoding;
        const struct rtsp_header_transport_t* transport;
        transport = rtsp_client_get_media_transport(priv->rtsp, i);
        encoding = rtsp_client_get_media_encoding(priv->rtsp, i);
        payload = rtsp_client_get_media_payload(priv->rtsp, i);

        if (RTSP_TRANSPORT_RTP_UDP == transport->transport) {
            int port[2] = {transport->rtp.u.server_port1, transport->rtp.u.server_port2};
            // only
            assert(0 == transport->multicast);  // unicast only
            assert(transport->rtp.u.client_port1 == priv->port[i][0]);
            assert(transport->rtp.u.client_port2 == priv->port[i][1]);
            if (*transport->source) {
                priv->receiver[i] = rtp_udp_receiver_create(priv->rtp[i], transport->source, port, payload, encoding);
            } else {
                char ip[65];
	            uint16_t rtspport;
                socket_getpeername(priv->socket, ip, &rtspport);
                priv->receiver[i] = rtp_udp_receiver_create(priv->rtp[i], ip, port, payload, encoding);
            }
        } else if (RTSP_TRANSPORT_RTP_TCP == transport->transport) {
            assert(transport->rtp.u.client_port1 == transport->interleaved1);
            assert(transport->rtp.u.client_port2 == transport->interleaved2);
            priv->receiver[i] = rtp_tcp_receiver_create(transport->interleaved1, transport->interleaved2, payload, encoding);
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
    tracef("param:%p media:%d nptbegin:%p nptend:%p scale:%p rtpinfo:%p count:%d",
        param, media, nptbegin, nptend, scale, rtpinfo, count);
    return 0;
}

static int onpause(void* param)
{
    tracef("param:%p", param);
    return 0;
}

static int onteardown(void* param)
{
    tracef("param:%p", param);
    return 0;
}

static void onrtp(void* param,
                  uint8_t channel,
                  const void* data,
                  uint16_t bytes)
{
    tracef("param:%p channel:%u data:%p bytes:%u", param, channel, data, bytes);
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

    // Get info
    struct uri_t* r = uri_parse(url, strlen(url));
    char usr[32], pwd[32];
    uri_userinfo(r, usr, sizeof(usr), pwd, sizeof(pwd));

    memset(priv, 0, sizeof(rtsp_client_priv_t));
    priv->transport = RTSP_TRANSPORT_RTP_UDP;
    priv->socket = socket_connect_host(r->host, r->port, -1);
    if (priv->socket == socket_invalid) {
        goto err;
    }
    socket_setnonblock(priv->socket, 1);
    priv->rtsp = rtsp_client_create(url, usr, pwd, &handler, priv);
    rtsp_client_describe(priv->rtsp);
    uri_free(r);

    thread_create(&priv->thread, rtsp_client_recv, priv);
    tracef("priv:%p", priv);
    return priv;

err:
    uri_free(r);
    free(priv);
    return NULL;
}

int rtsp_client_uninit(void* rtsp)
{
    int i;
    rtsp_client_priv_t* priv = rtsp;

    for (i = 0; i < 5; i++) {
        if (priv->receiver[i])
            priv->receiver[i]->free(priv->receiver[i]);
    }

    rtsp_client_teardown(priv->rtsp);
    rtsp_client_destroy(priv->rtsp);
    socket_close(priv->socket);
    thread_destroy(priv->thread);
    free(priv);

    return 0;
}
