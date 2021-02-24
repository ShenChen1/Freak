#ifndef __SDK_CFG_JSONB_C_API_H__
#define __SDK_CFG_JSONB_C_API_H__
#include "jsonb_c_type.h"
#include "inc/sdk_common.h"
#include "proto_vsf.h"
extern void jsonb_opt_sdk_sys_vb_pool_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint32_t u32BlkCnt;
    uint32_t u32Width;
    uint32_t u32Height;
} sdk_sys_vb_pool_info_t;
extern void jsonb_opt_sdk_sys_vb_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint32_t u32MaxPoolCnt;
    sdk_sys_vb_pool_info_t astCommPool[VB_MAX_COMM_POOLS];
} sdk_sys_vb_info_t;
extern void jsonb_opt_sdk_sys_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    sdk_sys_vb_info_t stVbConf;
} sdk_sys_info_t;
extern void jsonb_opt_sdk_vi_sensor_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint32_t enSnsType;
    int32_t s32SnsId;
    int32_t s32BusId;
    int32_t MipiDev;
} sdk_vi_sensor_info_t;
extern void jsonb_opt_sdk_vi_dev_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int32_t ViDev;
    uint32_t enWDRMode;
} sdk_vi_dev_info_t;
extern void jsonb_opt_sdk_vi_pipe_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int32_t aPipe[WDR_MAX_PIPE_NUM];
    uint32_t enMastPipeMode;
    uint32_t bMultiPipe;
    int32_t bVcNumCfged;
    uint32_t bIspBypass;
    uint32_t enPixFmt;
    uint32_t u32VCNum[WDR_MAX_PIPE_NUM];
} sdk_vi_pipe_info_t;
extern void jsonb_opt_sdk_vi_chn_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int32_t ViChn;
    uint32_t enPixFormat;
    uint32_t enDynamicRange;
    uint32_t enVideoFormat;
    uint32_t enCompressMode;
} sdk_vi_chn_info_t;
extern void jsonb_opt_sdk_vi_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    sdk_vi_sensor_info_t stSnsInfo;
    sdk_vi_dev_info_t stDevInfo;
    sdk_vi_pipe_info_t stPipeInfo;
    sdk_vi_chn_info_t stChnInfo;
} sdk_vi_info_t;
extern void jsonb_opt_sdk_vpss_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int32_t VpssGrp;
    int32_t ViPipe;
    int32_t ViChn;
    uint32_t abChnEnable[VPSS_MAX_PHY_CHN_NUM];
    int32_t aenSize[VPSS_MAX_PHY_CHN_NUM];
    int32_t au32Depth[VPSS_MAX_PHY_CHN_NUM];
    uint32_t enSnsSize;
} sdk_vpss_info_t;
extern void jsonb_opt_sdk_venc_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int32_t VencChn;
    int32_t VpssGrp;
    int32_t VpssChn;
    uint32_t u32Profile;
    uint32_t enPayLoad;
    uint32_t enSize;
    uint32_t bRcnRefShareBuf;
    uint32_t enRcMode;
    uint32_t enGopMode;
} sdk_venc_info_t;
extern void jsonb_opt_sdk_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int32_t s32SysNum;
    int32_t as32SysId[1];
    sdk_sys_info_t astSysInfo[1];
    int32_t s32ViNum;
    int32_t as32ViId[VI_MAX_DEV_NUM];
    sdk_vi_info_t astViInfo[VI_MAX_DEV_NUM];
    int32_t s32VpssNum;
    int32_t as32VpssId[VPSS_MAX_GRP_NUM];
    sdk_vpss_info_t astVpssInfo[VPSS_MAX_GRP_NUM];
    int32_t s32VencNum;
    int32_t as32VencId[VENC_MAX_CHN_NUM];
    sdk_venc_info_t astVencInfo[VENC_MAX_CHN_NUM];
} sdk_cfg_t;
#endif /* __SDK_CFG_JSONB_C_API_H__ */
