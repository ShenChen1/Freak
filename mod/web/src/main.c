#include "common.h"
#include "log.h"
#include "nnm.h"
#include "inc/cfg.h"
#include "inc/msgbox.h"
#include "web/svr_mgr.h"

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize, void *arg)
{
    *out = arg;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main()
{
    nnm_t rep = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    cfg_load(PROTO_WEB_CFG_PATH);
    msgbox_init();

    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = { __rep_recv, obuf };
    nnm_rep_create(PROTO_WEB_COM_NODE, &init, &rep);

    int i, total;
    web_svr_mgr_t *server = web_createSvrMgr();
    total = server->num(server);
    for (i = 0; i < total; i++) {
        proto_web_svr_cfg_t cfg = {.id = i};
        // get cfg
        server->get(server, &cfg);
        // set cfg
        server->set(server, &cfg);
    }
    server->destroy(server);

    while (1) {
        infof("keep alive");
        sleep(1);
    }

    nnm_rep_destory(rep);
    msgbox_deinit();
    log_deinit();

    return 0;
}