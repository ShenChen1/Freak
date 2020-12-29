#include "common.h"
#include "json-cfg.h"
#include "log.h"
#include "cfg/cfg.h"

static bsp_cfg_t s_cfg = {
    .dummy = {
        .num = { 2 },
        .cfgs[0] = {
            .value = 0x1234,
        },
        .caps[0] = {
            .value = 0x2345,
        },
        .cfgs[1] = {
            .value = 0x3456,
        },
        .caps[1] = {
            .value = 0x4567,
        },
    },
};

int cfg_load(const char *path)
{
    // try to read load from json file
    json_cfg_load(path, &s_cfg, sizeof(bsp_cfg_t), jsonb_opt_bsp_cfg_t);
    return 0;
}

void *cfg_get()
{
    return &s_cfg;
}

int cfg_save(const char *path)
{
    // save to json file
    return json_cfg_save(path, &s_cfg, sizeof(bsp_cfg_t), jsonb_opt_bsp_cfg_t);
}
