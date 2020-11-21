#include "common.h"
#include "json-cfg.h"
#include "log.h"
#include "media.h"
#include "cfg/cfg.h"

static vsf_cfg_t s_cfg = {
    .stream = {
        .num = 3,
        .streams[0] = {
            .id = 0,
            .enable = 1,
            .chn = 0,
            .subchn = 0,
            .encode = VIDEO_STREAM_TYPE_H264,
            .width = 1920,
            .height = 1080,
            .fps = 25,
        },
        .streams[1] = {
            .id = 1,
            .enable = 1,
            .chn = 0,
            .subchn = 1,
            .encode = VIDEO_STREAM_TYPE_H264,
            .width = 1280,
            .height = 720,
            .fps = 25,
        },
        .streams[2] = {
            .id = 2,
            .enable = 0,
            .chn = 0,
            .subchn = 2,
            .encode = VIDEO_STREAM_TYPE_JPEG,
            .width = 1920,
            .height = 1080,
            .fps = 1,
        },
    },
    .frame = {
        .num = 2,
        .frames[0] = {
            .id = 0,
            .enable = 0,
            .chn = 0,
            .subchn = 0,
            .format = VIDEO_FRAME_FORMAT_YUV420P_YVU,
            .width = 1920,
            .height = 1080,
            .fps = 10,
        },
        .frames[1] = {
            .id = 1,
            .enable = 0,
            .chn = 0,
            .subchn = 1,
            .format = VIDEO_FRAME_FORMAT_YUV420P_YVU,
            .width = 1280,
            .height = 720,
            .fps = 5,
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
