#include "common.h"
#include "json-cfg.h"
#include "log.h"
#include "media.h"
#include "cfg/cfg.h"

static vsf_cfg_t s_cfg = {
    .stream = {
        .num = { 3 },
        .cfgs[0] = {
            .id = 0,
            .enable = 1,
            .encode = VIDEO_STREAM_TYPE_H264,
            .width = 1920,
            .height = 1080,
            .fps = 25,
        },
        .caps[0] = {
            .id = 0,
            .chn = 0,
            .subchn = 0,
        },
        .cfgs[1] = {
            .id = 1,
            .enable = 1,
            .encode = VIDEO_STREAM_TYPE_H264,
            .width = 1280,
            .height = 720,
            .fps = 25,
        },
        .caps[1] = {
            .id = 1,
            .chn = 0,
            .subchn = 1,
        },
        .cfgs[2] = {
            .id = 2,
            .enable = 0,
            .encode = VIDEO_STREAM_TYPE_JPEG,
            .width = 1920,
            .height = 1080,
            .fps = 1,
        },
        .caps[2] = {
            .id = 2,
            .chn = 0,
            .subchn = 2,
        },
    },
    .frame = {
        .num = { 2 },
        .cfgs[0] = {
            .id = 0,
            .enable = 0,
            .format = VIDEO_FRAME_FORMAT_YUV420P_YVU,
            .width = 1920,
            .height = 1080,
            .fps = 10,
        },
        .caps[0] = {
            .id = 0,
            .chn = 0,
            .subchn = 1,
        },
        .cfgs[1] = {
            .id = 1,
            .enable = 0,
            .format = VIDEO_FRAME_FORMAT_YUV420P_YVU,
            .width = 1280,
            .height = 720,
            .fps = 5,
        },
        .caps[1] = {
            .id = 0,
            .chn = 0,
            .subchn = 2,
        },
    },
    .osd = {
        .num = { 1 },
        .cfgs[0] = {
            .id = 0,
            .enable = 1,
            .info = {
                .condition = "mask",
                .mask = {
                    .points[0] = {100, 100},
                    .points[1] = {400, 400},
                    .points[2] = {100, 400},
                    .points[3] = {400, 100},
                },
            },
        },
        .caps[0] = {
            .id = 0,
            .chn = 0,
            .subchn = 1,
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
