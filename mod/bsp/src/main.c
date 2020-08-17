#include <stdio.h>
#include <unistd.h>
#include "cfg/cfg.h"
#include "json-cfg.h"
#include "nnm.h"
#include "log.h"

gsf_bsp_t g_bsp_cfg = {};

int main()
{
    json_cfg_load("tmp", &g_bsp_cfg, sizeof(gsf_bsp_t), jsonb_opt_gsf_bsp_board_t);
    json_cfg_save("tmp", &g_bsp_cfg, sizeof(gsf_bsp_t), jsonb_opt_gsf_bsp_board_t);


    log_init(1);

    while (1) {
        infof("sleep 1");
        sleep(1);
    }

    log_deinit();
}