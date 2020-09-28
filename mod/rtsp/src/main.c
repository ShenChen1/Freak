#include "common.h"
#include "inc/msgbox.h"
#include "inc/cfg.h"
#include "inc/rtsp.h"
#include "nnm.h"
#include "log.h"
#include "ufifo.h"

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize, void *arg)
{
    *out = arg;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main()
{
    void *server = NULL;
    nnm_t rep = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    log_setlevel(LOG_LV_DEBUG);
    cfg_load(PROTO_RTSP_CFG_PATH);
    msgbox_init();
    server = rtsp_server_init("0.0.0.0", 1234);

    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = {__rep_recv, obuf};
    nnm_rep_create(PROTO_RTSP_COM_NODE, &init, &rep);

    while (1) {
        infof("keep alive");
        sleep(1);
    }

    nnm_rep_destory(rep);
    rtsp_server_uninit(server);
    msgbox_deinit();
    log_deinit();

    return 0;
}