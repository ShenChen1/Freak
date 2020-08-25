#include "librtsp/rtsp-server-aio.h"
#include "librtp/rtp-profile.h"
#include "cstringext.h"
#include "ctypedef.h"
#include "ntp-time.h"
#include "uri-parse.h"
#include "urlcodec.h"

int rtsp_uri_parse(const char* uri, char* path)
{
    char path1[256];
    struct uri_t* r = uri_parse(uri, strlen(uri));
    if (!r)
        return -1;

    url_decode(r->path, strlen(r->path), path1, sizeof(path1));
    strncpy(path, path1, 256);
    uri_free(r);
    return 0;
}

void* rtsp_server_init(const char* ip, int port)
{
    struct aio_rtsp_handler_t* handler =
        calloc(1, sizeof(struct aio_rtsp_handler_t));

    void* l = rtsp_server_listen(ip, port, handler, NULL);
    if (l == NULL) {
        free(handler);
    }

    return l;
}

int rtsp_server_uninit(void* st)
{
    struct aio_rtsp_handler_t* handler = (struct aio_rtsp_handler_t*)st;

    rtsp_server_unlisten(st);

    free(handler);
    return 0;
}