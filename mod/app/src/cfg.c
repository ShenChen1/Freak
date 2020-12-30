#include "common.h"
#include "json-cfg.h"
#include "log.h"
#include "cfg/cfg.h"

static app_cfg_t s_cfg = {
    .alginfo = {
        .num = { 1 },
        .cfgs[0] = {
            .id = 0,
            .type = ALG_TYPE_FR,
            .enable = 1,
            .algpath = "./data/",
        },
        .caps[0] = {
            .id = 0,
            .dummy = 0x1234,
        },
    },
};

int cfg_load(const char *path)
{
    // try to read load from json file
    json_cfg_load(path, &s_cfg, sizeof(app_cfg_t), jsonb_opt_app_cfg_t);
    return 0;
}

void *cfg_get()
{
    return &s_cfg;
}

int cfg_save(const char *path)
{
    // save to json file
    return json_cfg_save(path, &s_cfg, sizeof(app_cfg_t), jsonb_opt_app_cfg_t);
}
