#include "common.h"
#include "log.h"
#include "msgbox.h"
#include "nnm.h"
#include "proto.h"
#include "inc/cfg.h"
#include "bsp/dummy.h"

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize, void *arg)
{
    *out = arg;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main()
{
    nnm_t rep = NULL;
    nnm_t pub = NULL;

    log_init(PROTO_LOG_COM_NODE, true);
    cfg_load(PROTO_BSP_CFG_PATH);

    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = { __rep_recv, obuf };
    nnm_rep_create(PROTO_BSP_COM_NODE, &init, &rep);
    nnm_pub_create(PROTO_BSP_PUB_NODE, &pub);

    int i, total;
    total = bsp_getDummyNum_r();
    for (i = 0; i < total; i++) {
        bsp_dummy_t *dummy = bsp_createDummy_r(i);
        proto_bsp_dummy_cfg_t cfg = {};
        // get cfg
        dummy->get(dummy, &cfg);
        // set cfg
        dummy->set(dummy, &cfg);
        // delete
        dummy->destroy(dummy);
    }

    while (1) {
        infof("keep alive");
        sleep(1);
    }

    nnm_rep_destory(rep);
    nnm_pub_destory(pub);
    log_deinit();

    return 0;
}