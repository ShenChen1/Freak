#include <stdio.h>
#include "cfg/cfg.h"
#include "json-cfg.h"

gsf_bsp_t g_bsp_cfg = {};

int main()
{
    json_cfg_load("tmp", &g_bsp_cfg, sizeof(gsf_bsp_t), jsonb_opt_gsf_bsp_board_t);
    json_cfg_save("tmp", &g_bsp_cfg, sizeof(gsf_bsp_t), jsonb_opt_gsf_bsp_board_t);
}