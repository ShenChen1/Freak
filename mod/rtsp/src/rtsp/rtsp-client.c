#define OS_LINUX
#include "cstringext.h"
#include "ctypedef.h"
#include "librtsp/rtsp-client.h"
#include "libaio/aio-worker.h"
#include "ntp-time.h"
#include "rtp-media.h"
#include "rtp-transport.h"
#include "uri-parse.h"
#include "urlcodec.h"
#include "sockutil.h"

struct rtsp_session_t {
    socket_t socket;
    struct rtp_media_t* media;
};

void* rtsp_client_init(const char* url)
{
    struct rtsp_session_t *priv = NULL;
    struct rtsp_client_handler_t handler = {};

#if 0
    handler.send = rtsp_client_send;
    handler.rtpport = rtpport;
    handler.ondescribe = ondescribe;
    handler.onsetup = onsetup;
    handler.onplay = onplay;
    handler.onpause = onpause;
    handler.onteardown = onteardown;
    handler.onrtp = onrtp;
#endif

    priv->socket = socket_connect_host(url, 443, 2000);
    return rtsp_client_create(url, NULL, NULL, &handler, priv);
}

int rtsp_client_uninit(void* rtsp)
{
    rtsp_client_destroy(rtsp);
    return 0;
}
