#include "common.h"
#include "cfg/cfg.h"
#include "json-cfg.h"
#include "log.h"

static bsp_cfg_t s_cfg = {
    .board = {
        .model = "IPC",
        .sn = "123456",
        .chip = "HI3516DV300",
    },
    .dummy[0] = {
        .value = 0xdeadbeef,
    },
};

int cfg_load(const char *path)
{
    // try to read load from json file
    json_cfg_load(path, &s_cfg, sizeof(bsp_cfg_t), jsonb_opt_bsp_cfg_t);
    return 0;
}

void *cfg_get(type)
{
    return &s_cfg;
}

int cfg_save(const char *path)
{
    // save to json file
    return json_cfg_save(path, &s_cfg, sizeof(bsp_cfg_t), jsonb_opt_bsp_cfg_t);
}

