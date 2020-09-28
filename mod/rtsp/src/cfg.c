#include "common.h"
#include "cfg/cfg.h"
#include "json-cfg.h"
#include "log.h"

static rtsp_cfg_t s_cfg = {

};

int cfg_load(const char *path)
{
    // try to read load from json file
    json_cfg_load(path, &s_cfg, sizeof(rtsp_cfg_t), jsonb_opt_rtsp_cfg_t);
    return 0;
}

void *cfg_get(type)
{
    return &s_cfg;
}

int cfg_save(const char *path)
{
    // save to json file
    return json_cfg_save(path, &s_cfg, sizeof(rtsp_cfg_t), jsonb_opt_rtsp_cfg_t);
}

