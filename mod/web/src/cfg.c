#include "common.h"
#include "json-cfg.h"
#include "log.h"
#include "cfg/cfg.h"

static web_cfg_t s_cfg = {
    .http = {
        .port = 8080,
    },
};

int cfg_load(const char *path)
{
    // try to read load from json file
    json_cfg_load(path, &s_cfg, sizeof(web_cfg_t), jsonb_opt_web_cfg_t);
    return 0;
}

void *cfg_get()
{
    return &s_cfg;
}

int cfg_save(const char *path)
{
    // save to json file
    return json_cfg_save(path, &s_cfg, sizeof(web_cfg_t), jsonb_opt_web_cfg_t);
}
