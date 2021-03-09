#include "common.h"
#include "log.h"
#include "msgbox.h"
#include "nnm.h"
#include "proto.h"
#include "inc/cfg.h"
#include "app/alg_mgr.h"

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize, void *arg)
{
    *out = arg;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main()
{
    nnm_t rep = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    cfg_load(PROTO_APP_CFG_PATH);
    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = { __rep_recv, obuf };
    nnm_rep_create(PROTO_APP_COM_NODE, &init, &rep);

    // init start
    int i, total;
    app_alg_mgr_t *alg = app_createAlgMgr_r();
    total = alg->num(alg);
    for (i = 0; i < total; i++) {
        proto_app_alg_cfg_t cfg = {.id = i};
        alg->get(alg, &cfg);
        alg->set(alg, &cfg);
    }
    alg->destroy(alg);

    while (1) {
        infof("keep alive");
        sleep(1);
    }

    nnm_rep_destory(rep);
    log_deinit();

    return 0;
}
