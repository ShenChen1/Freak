#include "cfg/cfg.h"
#include "common.h"
#include "json-cfg.h"
#include "log.h"
#include "media.h"

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
            .width = 640,
            .height = 640,
            .fps = 5,
        },
        .caps[1] = {
            .id = 1,
            .chn = 0,
            .subchn = 2,
        },
    },
    .osd = {
        .num = { VSF_OSD_MAX },
        .cfgs[0] = {
            .id = 0,
            .enable = 1,
            .info = {
                .condition = "mask",
                .mask = {
                    .color = 0x0000ffff,
                    .points[0] = {100, 100},
                    .points[1] = {400, 400},
                    .points[2] = {100, 400},
                    .points[3] = {400, 100},
                },
            },
        },
        .caps[0] = {
            .id = 0,
        },
        .cfgs[1] = {
            .id = 1,
            .enable = 1,
            .info = {
                .condition = "text",
                .text = {
                    .point = {4096, 4096},
                    .size = 48,
                    .color = 0x0100ff00,
                    .text = "123abcABC",
                },
            },
        },
        .caps[1] = {
            .id = 1,
        },
        .cfgs[3] = {
            .id = 3,
            .enable = 1,
            .info = {
                .condition = "mask",
                .mask = {
                    .color = 0x0000ffff,
                    .points[0] = {6000, 6000},
                    .points[1] = {6600, 6600},
                    .points[2] = {6000, 6600},
                    .points[3] = {6600, 6000},
                },
            },
        },
        .caps[3] = {
            .id = 3,
        },
        .cfgs[4] = {
            .id = 4,
            .enable = 1,
            .info = {
                .condition = "text",
                .text = {
                    .point = {2048, 4096},
                    .size = 48,
                    .color = 0x01ff0000,
                    .text = "abc",
                },
            },
        },
        .caps[4] = {
            .id = 4,
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
