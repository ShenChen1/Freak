#include "cfg/sdk_cfg.h"
#include "common.h"
#include "json-cfg.h"

static sdk_cfg_t s_cfg = {

};

static int sdk_cfg_load(const char *path)
{
    // try to read load from json file
    json_cfg_load(path, &s_cfg, sizeof(sdk_cfg_t), jsonb_opt_sdk_cfg_t);
    return 0;
}

void *sdk_cfg_get()
{
    return &s_cfg;
}

static void before101()
{
    sdk_cfg_load();
}