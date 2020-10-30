#include "common.h"
#include "cfg/sdk_cfg.h"
#include "json-cfg.h"

static sdk_cfg_t s_cfg = {

};

int sdk_cfg_load(const char *path)
{
    // try to read load from json file
    json_cfg_load(path, &s_cfg, sizeof(sdk_cfg_t), jsonb_opt_sdk_cfg_t);
    return 0;
}

void *sdk_cfg_get()
{
    return &s_cfg;
}

int sdk_cfg_save(const char *path)
{
    // save to json file
    return json_cfg_save(path, &s_cfg, sizeof(sdk_cfg_t), jsonb_opt_sdk_cfg_t);
}