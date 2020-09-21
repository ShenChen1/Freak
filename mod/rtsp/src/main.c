#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "inc/msgbox.h"
#include "inc/cfg.h"
#include "inc/rtsp.h"
#include "nnm.h"
#include "log.h"

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize)
{
    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];

    *out = obuf;
    return msgbox_do_handler(in, isize, *out, osize);
}

static int __frame(void* param,
                   const char* encoding,
                   const void* packet,
                   int bytes,
                   uint32_t time,
                   int flags)
{
    tracef("client:%p", *(void **)param);
    return 0;
}

int main()
{
    void *server = NULL;
    void *client1 = NULL;
    void *client2 = NULL;
    rtsp_client_callback_t cb = {&client1, __frame};
    nnm_t rep = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    log_setlevel(LOG_LV_DEBUG);
    cfg_load(PROTO_RTSP_CFG_PATH);
    msgbox_init();
    server = rtsp_server_init("0.0.0.0", 1234);
    nnm_rep_create(PROTO_RTSP_COM_NODE, __rep_recv, &rep);

    if (1) {
        infof("keep alive");
        cb.param = &client1;
        client1 = rtsp_client_init("rtsp://admin@127.0.0.1:1234/test", RTSP_PROTOCOL_TCP, &cb);
        cb.param = &client2;
        client2 = rtsp_client_init("rtsp://admin@127.0.0.1:1234/test", RTSP_PROTOCOL_UDP, &cb);
        sleep(5);
        rtsp_client_uninit(client1);
        rtsp_client_uninit(client2);
    }

    nnm_rep_destory(rep);
    rtsp_server_uninit(server);
    msgbox_deinit();
    log_deinit();

    return 0;
}