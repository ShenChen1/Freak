#include "cfg/sdk_cfg.h"
#include "common.h"
#include "json-cfg.h"

static const char s_path[] = "/tmp/sdk-config.json";
static sdk_cfg_t s_cfg = {
    .s32SysNum = 1,
    .as32SysId = {0},
    .astSysInfo[0] = {
        .stVbConf = {
            .u32MaxPoolCnt = 2,
            .astCommPool[0] = {
                .u32BlkCnt = 10,
                .enSize = PIC_1080P,
            },
            .astCommPool[1] = {
                .u32BlkCnt = 10,
                .enSize = PIC_720P,
            },
        },
    },
    .s32ViNum = 1,
    .as32ViId = {0},
    .astViInfo[0] = {
        .stSnsInfo = {
            .enSnsType = SENSOR0_TYPE,
            .s32SnsId = 0,
            .s32BusId = 0,
            .MipiDev = 0,
        },
        .stPipeInfo = {
            .aPipe = {0, -1, -1, -1},
            .enMastPipeMode = VI_OFFLINE_VPSS_OFFLINE,
            .bMultiPipe = HI_FALSE,
            .bVcNumCfged = HI_FALSE,
            .bIspBypass = HI_FALSE,
            .enPixFmt = PIXEL_FORMAT_YVU_SEMIPLANAR_420,
        },
        .stDevInfo = {
            .ViDev = 0,
            .enWDRMode = WDR_MODE_NONE,
        },
        .stChnInfo = {
            .ViChn = 0,
            .enPixFormat = PIXEL_FORMAT_YVU_SEMIPLANAR_420,
            .enDynamicRange = DYNAMIC_RANGE_SDR8,
            .enVideoFormat = VIDEO_FORMAT_LINEAR,
            .enCompressMode = COMPRESS_MODE_SEG,
        },
    },
    .s32VpssNum = 1,
    .as32VpssId = {0},
    .astVpssInfo[0] = {
        .VpssGrp = 0,
        .ViPipe = 0,
        .ViChn = 0,
        .abChnEnable = {1, 0},
        .aenSize = {PIC_1080P, PIC_720P},
        .enSnsType = SENSOR0_TYPE,
    },
    .s32VencNum = 3,
    .as32VencId = {0, 1, 2},
    .astVencInfo[0] = {
        .VencChn = 0,
        .VpssGrp = 0,
        .VpssChn = 0,
        .u32Profile = 0,
        .enPayLoad = PT_H264,
        .enSize = PIC_1080P,
        .bRcnRefShareBuf = HI_TRUE,
        .enRcMode = SAMPLE_RC_CBR,
        .enGopMode = VENC_GOPMODE_NORMALP,
    },
    .astVencInfo[1] = {
        .VencChn = 1,
        .VpssGrp = 0,
        .VpssChn = 0,
        .u32Profile = 0,
        .enPayLoad = PT_H264,
        .enSize = PIC_720P,
        .bRcnRefShareBuf = HI_TRUE,
        .enRcMode = SAMPLE_RC_CBR,
        .enGopMode = VENC_GOPMODE_NORMALP,
    },
    .astVencInfo[2] = {
        .VencChn = 2,
        .VpssGrp = 0,
        .VpssChn = 0,
        .u32Profile = 0,
        .enPayLoad = PT_JPEG,
        .enSize = PIC_1080P,
        .bRcnRefShareBuf = HI_TRUE,
        .enRcMode = SAMPLE_RC_CBR,
        .enGopMode = VENC_GOPMODE_NORMALP,
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
