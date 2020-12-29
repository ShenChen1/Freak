#include "common.h"
#include "log.h"
#include "nnm.h"
#include "inc/cfg.h"
#include "inc/appalg.h"
#include "inc/msgbox.h"
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
    msgbox_init();
    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = { __rep_recv, obuf };
    nnm_rep_create(PROTO_APP_COM_NODE, &init, &rep);

    // init start
	app_alg_mgr_t *app_mgr = app_createAlgMgr();
	proto_app_alg_cfg_t cfg;
	app_mgr->get(app_mgr,&cfg);
	app_mgr->set(app_mgr,&cfg);
    app_mgr->destroy(app_mgr);

    while (1) {
        infof("keep alive");
        sleep(15);
    }

    nnm_rep_destory(rep);
    msgbox_deinit();
    log_deinit();

    return 0;
}
