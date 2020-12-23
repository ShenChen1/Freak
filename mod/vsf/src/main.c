#include "common.h"
#include "nnm.h"
#include "log.h"
#include "inc/msgbox.h"
#include "inc/cfg.h"

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize, void *arg)
{
    *out = arg;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main()
{
    nnm_t rep = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    cfg_load(PROTO_VSF_CFG_PATH);
    msgbox_init();

    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = { __rep_recv, obuf };
    nnm_rep_create(PROTO_VSF_COM_NODE, &init, &rep);

    while (1) {
        infof("keep alive");
        sleep(1);
    }

    nnm_rep_destory(rep);
    msgbox_deinit();
    log_deinit();

    return 0;
}