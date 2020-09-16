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

int main()
{
    void *server = NULL;
    void *client = NULL;
    nnm_t rep = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    log_setlevel(LOG_LV_DEBUG);
    cfg_load(PROTO_RTSP_CFG_PATH);
    msgbox_init();
    server = rtsp_server_init("0.0.0.0", 1234);
    nnm_rep_create(PROTO_RTSP_COM_NODE, __rep_recv, &rep);

    if (1) {
        infof("keep alive");
        client = rtsp_client_init("rtsp://admin@127.0.0.1:1234/test");
        sleep(5);
        rtsp_client_uninit(client);
    }

    nnm_rep_destory(rep);
    rtsp_server_uninit(server);
    msgbox_deinit();
    log_deinit();

    return 0;
}