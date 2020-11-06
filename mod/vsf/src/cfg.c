#include "common.h"
#include "json-cfg.h"
#include "log.h"
#include "media.h"
#include "cfg/cfg.h"

static vsf_cfg_t s_cfg = {
    .stream = {
        .num = 1,
        .streams[0] = {
            .id = 0,
            .enable = 1,
            .chn = 0,
            .subchn = 0,
            .encode = VIDEO_ENCODE_TYPE_H264,
            .width = 1920,
            .height = 1080,
            .fps = 25,
        },
    },
};

int cfg_load(const char *path)
{
    // try to read load from json file
    json_cfg_load(path, &s_cfg, sizeof(vsf_cfg_t), jsonb_opt_vsf_cfg_t);
    return 0;
}

void *cfg_get()
{
    return &s_cfg;
}

int cfg_save(const char *path)
{
    // save to json file
    return json_cfg_save(path, &s_cfg, sizeof(vsf_cfg_t), jsonb_opt_vsf_cfg_t);
}
