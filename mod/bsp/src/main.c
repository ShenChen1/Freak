#include <stdio.h>
#include <unistd.h>
#include "cfg/cfg.h"
#include "inc/msgbox.h"
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
    nnm_t rep = NULL;
    nnm_t pub = NULL;

    json_cfg_load(PROTO_BSP_CFG_PATH, &s_cfg, sizeof(bsp_cfg_t), jsonb_opt_bsp_cfg_t);
    json_cfg_save(PROTO_BSP_CFG_PATH, &s_cfg, sizeof(bsp_cfg_t), jsonb_opt_bsp_cfg_t);

    log_init(1);
    msgbox_init();
    nnm_pub_create(PROTO_BSP_PUB_NODE, &pub);
    nnm_rep_create(PROTO_BSP_COM_NODE, bsp_rep_recv, &rep);

    while (1) {
        errorf("sleep 1");
        sleep(1);
    }

    nnm_rep_destory(rep);
    nnm_pub_destory(pub);
    msgbox_deinit();
    log_deinit();

    return 0;
}