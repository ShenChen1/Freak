#include "common.h"
#include "inc/cfg.h"
#include "inc/msgbox.h"
#include "inc/rtsp-ctrl.h"
#include "inc/rtsp.h"
#include "log.h"
#include "nnm.h"
#include "time64.h"
#include "ufifo.h"

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize, void *arg)
{
    *out = arg;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main(int argc, char *argv[])
{
    void *server = NULL;
    nnm_t rep    = NULL;
    nnm_t req    = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    log_setlevel(LOG_LV_INFO);
    cfg_load(PROTO_RTSP_CFG_PATH);
    msgbox_init();
    server = rtsp_server_init("0.0.0.0", *cfg_get_member(port));

    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = { __rep_recv, obuf };
    nnm_rep_create(PROTO_RTSP_COM_NODE, &init, &rep);
    nnm_req_create(PROTO_RTSP_COM_NODE, &req);

    rtsp_ctrl_init();
    while (1) {
        infof("keep alive");
        rtsp_ctrl_thread(NULL);
    }
    rtsp_ctrl_uninit();

    nnm_req_destory(req);
    nnm_rep_destory(rep);
    rtsp_server_uninit(server);
    msgbox_deinit();
    log_deinit();

    return 0;
}