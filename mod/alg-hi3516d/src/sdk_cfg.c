#include "cfg/sdk_cfg.h"
#include "common.h"
#include "json-cfg.h"

static const char s_path[] = "/tmp/sdk-alg-config.json";
static sdk_cfg_t s_cfg = {

};

static int sdk_cfg_load(const char *path)
{
    // try to read load from json file
    json_cfg_load(path, &s_cfg, sizeof(sdk_cfg_t), jsonb_opt_sdk_cfg_t);
    return 0;
}

static int sdk_cfg_save(const char *path)
{
    // save to json file
    return json_cfg_save(path, &s_cfg, sizeof(sdk_cfg_t), jsonb_opt_sdk_cfg_t);
}

void *sdk_cfg_get()
{
    return &s_cfg;
}

static void __attribute__((constructor(SDK_CFG_PRIORITY))) sdk_cfg_constructor()
{
    sdk_cfg_load(s_path);
}

static void __attribute__((destructor(SDK_CFG_PRIORITY))) sdk_cfg_destructor()
{
    sdk_cfg_save(s_path);
}
