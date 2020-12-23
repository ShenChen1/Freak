#include "common.h"
#include "log.h"
#include "nnm.h"
#include "inc/cfg.h"
#include "bsp/dummy.h"
#include "inc/msgbox.h"

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
    msgbox_init();

    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = { __rep_recv, obuf };
    nnm_rep_create(PROTO_BSP_COM_NODE, &init, &rep);
    nnm_pub_create(PROTO_BSP_PUB_NODE, &pub);

    int i;
    int total = getDummyNum();
    for (i = 0; i < total; i++) {
        dummy_t *dummy = createDummy(i);
        // get cap
        proto_bsp_dummy_cap_t cap;
        dummy->cap(dummy, &cap);
        // set cfg
        proto_bsp_dummy_cfg_t cfg;
        cfg.value = cap.value;
        dummy->set(dummy, &cfg);
        // get cfg
        dummy->get(dummy, &cfg);
        assert(cfg.value == cap.value);
        // delete
        dummy->destroy(dummy);
    }

    while (1) {
        infof("keep alive");
        sleep(1);
    }

    nnm_rep_destory(rep);
    nnm_pub_destory(pub);
    msgbox_deinit();
    log_deinit();

    return 0;
}