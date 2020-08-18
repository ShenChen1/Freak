#include <stdio.h>
#include <unistd.h>
#include "cfg/cfg.h"
#include "inc/msgbox.h"
#include "json-cfg.h"
#include "node.h"
#include "path.h"
#include "nnm.h"
#include "log.h"

static gsf_bsp_t s_bsp_cfg = {
    .board = {
        .model = "IPC",
        .sn = "123456",
        .chip = "HI3516DV300",
    },
};

static int bsp_rep_recv(void *in, size_t isize, void **out, size_t *osize)
{
    uint8_t obuf[512];

    *out = obuf;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main()
{
    nnm_t rep = NULL;
    nnm_t pub = NULL;

    json_cfg_load(BSP_CFG_PATH, &s_bsp_cfg, sizeof(gsf_bsp_t), jsonb_opt_gsf_bsp_board_t);
    json_cfg_save(BSP_CFG_PATH, &s_bsp_cfg, sizeof(gsf_bsp_t), jsonb_opt_gsf_bsp_board_t);

    log_init(1);
    msgbox_init();
    nnm_pub_create(GSF_BSP_PUB_NODE, &pub);
    nnm_rep_create(GSF_BSP_COM_NODE, bsp_rep_recv, &rep);

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