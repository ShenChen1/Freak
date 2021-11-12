/*
 * @Author: your name
 * @Date: 2021-11-05 11:20:27
 * @LastEditTime: 2021-11-12 17:07:00
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \Freak\mod\vsf-rv1126\src\sdk_cfg.c
 */
#include "cfg/sdk_cfg.h"
#include "common.h"
#include "json-cfg.h"

static const char s_path[] = "/tmp/sdk-vsf-config.json";
static sdk_cfg_t s_cfg = {
    .s32ViNum = 1,
    .as32ViId = {0},
    .astViInfo[0] = {
        .pipeId = 0,
        .chnId = 0,
        .stChnInfo = {
            .pcVideoNode = {"/dev/video30"},
            .u32Width = 1920,
            .u32Height = 1080,
            .enPixFmt = IMAGE_TYPE_NV12,
            .u32BufCnt = 3,
            .enBufType = VI_CHN_BUF_TYPE_MMAP,
            .enWorkMode = VI_WORK_MODE_NORMAL,
        },
    },
    .s32VencNum = 1,
    .as32VencId = {0},
    .astVencInfo[0] = {
        .DevId = 0,
        .ViChnId = 0,
        .ChnId = 0,
        .enType = RK_CODEC_TYPE_H264,
        .enRcMode = VENC_RC_MODE_H264CBR,
        .Gop = 30,
        .PicWidth = 1920,
        .PicHeight = 1080,
        .SrcFrameRateDen = 1,
        .SrcFrameRateNum = 30,
        .DstFrameRateDen = 1,
        .DstFrameRateNum = 30,
        .imageType = IMAGE_TYPE_NV12,
        .Profile = 66,
    },
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
