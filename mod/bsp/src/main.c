#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "cfg/cfg.h"
#include "inc/msgbox.h"
#include "inc/dummy.h"
#include "json-cfg.h"
#include "nnm.h"
#include "log.h"

static bsp_cfg_t s_cfg = {
    .board = {
        .model = "IPC",
        .sn = "123456",
        .chip = "HI3516DV300",
    },
    .dummy = {
        .value = 0xdeadbeef,
    },
};

static int bsp_rep_recv(void *in, size_t isize, void **out, size_t *osize)
{
    uint8_t obuf[PROTO_PACKAGE_MAXSIZE];

    *out = obuf;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main()
{
    int i, total;
    nnm_t rep = NULL;
    nnm_t pub = NULL;

    json_cfg_load(PROTO_BSP_CFG_PATH, &s_cfg, sizeof(bsp_cfg_t), jsonb_opt_bsp_cfg_t);
    json_cfg_save(PROTO_BSP_CFG_PATH, &s_cfg, sizeof(bsp_cfg_t), jsonb_opt_bsp_cfg_t);

    log_init(1);

    total = getDummyResourceNum();
    for (i = 0; i < total; i++) {
        createDummy(i);
    }

    msgbox_init();
    nnm_pub_create(PROTO_BSP_PUB_NODE, &pub);
    nnm_rep_create(PROTO_BSP_COM_NODE, bsp_rep_recv, &rep);

    nnm_t req = NULL;
    nnm_req_create(PROTO_BSP_COM_NODE, &req);
    while (1) {
        errorf("sleep 1");
        sleep(1);

        uint8_t ibuf[PROTO_PACKAGE_MAXSIZE];
        uint8_t *obuf = NULL;
        size_t osize = 0;
        proto_bsp_dummy_t data = {.value = i++};

        infof("set value");
        proto_package_fill(ibuf, 1, PROTO_ACTION_SET, PROTP_BSP_KEY_DUMMY, &data, sizeof(proto_bsp_dummy_t));
        nnm_req_exchange(req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
        assert(osize == sizeof(proto_header_t));
        infof("obuf:%p osize:%zu", obuf, osize);
        memcpy(ibuf, obuf, osize);
        nnm_free(obuf);

        infof("get value");
        proto_package_fill(ibuf, 1, PROTO_ACTION_GET, PROTP_BSP_KEY_DUMMY, &data, 0);
        nnm_req_exchange(req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
        assert(osize == sizeof(proto_header_t) + sizeof(proto_bsp_dummy_t));
        infof("value = %d", ((proto_bsp_dummy_t *)proto_package_data(obuf))->value);
        nnm_free(obuf);
    }

    nnm_rep_destory(rep);
    nnm_pub_destory(pub);
    msgbox_deinit();
    log_deinit();

    return 0;
}