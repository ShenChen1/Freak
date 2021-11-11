#ifndef __SDK_CFG_JSONB_C_API_H__
#define __SDK_CFG_JSONB_C_API_H__
#include "jsonb_c_type.h"
#include "inc/sdk_common.h"
extern void jsonb_opt_sdk_vi_chn_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    char pcVideoNode[16];
    uint32_t u32Width;
    uint32_t u32Height;
    uint32_t enPixFmt;
    uint32_t u32BufCnt;
    uint32_t enBufType;
    uint32_t enWorkMode;
} sdk_vi_chn_info_t;
extern void jsonb_opt_sdk_vi_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint32_t pipeId;
    uint32_t chnId;
    sdk_vi_chn_info_t stChnInfo;
} sdk_vi_info_t;
extern void jsonb_opt_sdk_venc_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint32_t DevId;
    uint32_t ViChnId;
    uint32_t ChnId;
    uint32_t enType;
    uint32_t enRcMode;
    uint32_t PicWidth;
    uint32_t PicHeight;
    uint32_t Gop;
    uint32_t SrcFrameRateDen;
    uint32_t SrcFrameRateNum;
    uint32_t DstFrameRateDen;
    uint32_t DstFrameRateNum;
    uint32_t imageType;
    uint32_t Profile;
} sdk_venc_info_t;
extern void jsonb_opt_sdk_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int32_t s32ViNum;
    int32_t as32ViId[4];
    sdk_vi_info_t astViInfo[4];
    int32_t s32VencNum;
    int32_t as32VencId[4];
    sdk_venc_info_t astVencInfo[4];
} sdk_cfg_t;
#endif /* __SDK_CFG_JSONB_C_API_H__ */
