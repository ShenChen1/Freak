#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "sdk_cfg.h"
void jsonb_opt_sdk_sys_vb_pool_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_sys_vb_pool_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "u32BlkCnt");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "u32BlkCnt", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->u32BlkCnt, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enSize");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enSize", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enSize, sizeof(uint32_t));
}
}
void jsonb_opt_sdk_sys_vb_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_sys_vb_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "u32MaxPoolCnt");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "u32MaxPoolCnt", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->u32MaxPoolCnt, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "astCommPool");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "astCommPool", json_child);
    }
    const size_t array_size_list[] = {VB_MAX_COMM_POOLS,0};
    jsonb_opt_array(opt, json_child, element->astCommPool, sizeof(sdk_sys_vb_pool_info_t) * 1 * VB_MAX_COMM_POOLS, array_size_list, jsonb_opt_sdk_sys_vb_pool_info_t);
}
}
void jsonb_opt_sdk_sys_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_sys_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "stVbConf");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "stVbConf", json_child);
    }
    jsonb_opt_sdk_sys_vb_info_t(opt, json_child, &element->stVbConf, sizeof(sdk_sys_vb_info_t));
}
}
void jsonb_opt_sdk_vi_sensor_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_vi_sensor_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enSnsType");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enSnsType", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enSnsType, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "s32SnsId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "s32SnsId", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->s32SnsId, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "s32BusId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "s32BusId", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->s32BusId, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "MipiDev");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "MipiDev", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->MipiDev, sizeof(int32_t));
}
}
void jsonb_opt_sdk_vi_dev_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_vi_dev_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "ViDev");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "ViDev", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->ViDev, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enWDRMode");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enWDRMode", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enWDRMode, sizeof(uint32_t));
}
}
void jsonb_opt_sdk_vi_pipe_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_vi_pipe_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "aPipe");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "aPipe", json_child);
    }
    const size_t array_size_list[] = {WDR_MAX_PIPE_NUM,0};
    jsonb_opt_array(opt, json_child, element->aPipe, sizeof(int32_t) * 1 * WDR_MAX_PIPE_NUM, array_size_list, jsonb_opt_int32_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enMastPipeMode");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enMastPipeMode", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enMastPipeMode, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "bMultiPipe");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "bMultiPipe", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->bMultiPipe, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "bVcNumCfged");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "bVcNumCfged", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->bVcNumCfged, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "bIspBypass");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "bIspBypass", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->bIspBypass, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enPixFmt");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enPixFmt", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enPixFmt, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "u32VCNum");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "u32VCNum", json_child);
    }
    const size_t array_size_list[] = {WDR_MAX_PIPE_NUM,0};
    jsonb_opt_array(opt, json_child, element->u32VCNum, sizeof(uint32_t) * 1 * WDR_MAX_PIPE_NUM, array_size_list, jsonb_opt_uint32_t);
}
}
void jsonb_opt_sdk_vi_chn_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_vi_chn_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "ViChn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "ViChn", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->ViChn, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enPixFormat");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enPixFormat", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enPixFormat, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enDynamicRange");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enDynamicRange", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enDynamicRange, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enVideoFormat");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enVideoFormat", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enVideoFormat, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enCompressMode");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enCompressMode", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enCompressMode, sizeof(uint32_t));
}
}
void jsonb_opt_sdk_vi_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_vi_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "stSnsInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "stSnsInfo", json_child);
    }
    jsonb_opt_sdk_vi_sensor_info_t(opt, json_child, &element->stSnsInfo, sizeof(sdk_vi_sensor_info_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "stDevInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "stDevInfo", json_child);
    }
    jsonb_opt_sdk_vi_dev_info_t(opt, json_child, &element->stDevInfo, sizeof(sdk_vi_dev_info_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "stPipeInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "stPipeInfo", json_child);
    }
    jsonb_opt_sdk_vi_pipe_info_t(opt, json_child, &element->stPipeInfo, sizeof(sdk_vi_pipe_info_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "stChnInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "stChnInfo", json_child);
    }
    jsonb_opt_sdk_vi_chn_info_t(opt, json_child, &element->stChnInfo, sizeof(sdk_vi_chn_info_t));
}
}
void jsonb_opt_sdk_vpss_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_vpss_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "VpssGrp");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "VpssGrp", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->VpssGrp, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "ViPipe");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "ViPipe", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->ViPipe, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "ViChn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "ViChn", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->ViChn, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "abChnEnable");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "abChnEnable", json_child);
    }
    const size_t array_size_list[] = {VPSS_MAX_PHY_CHN_NUM,0};
    jsonb_opt_array(opt, json_child, element->abChnEnable, sizeof(uint32_t) * 1 * VPSS_MAX_PHY_CHN_NUM, array_size_list, jsonb_opt_uint32_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "aenSize");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "aenSize", json_child);
    }
    const size_t array_size_list[] = {VPSS_MAX_PHY_CHN_NUM,0};
    jsonb_opt_array(opt, json_child, element->aenSize, sizeof(int32_t) * 1 * VPSS_MAX_PHY_CHN_NUM, array_size_list, jsonb_opt_int32_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "au32Depth");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "au32Depth", json_child);
    }
    const size_t array_size_list[] = {VPSS_MAX_PHY_CHN_NUM,0};
    jsonb_opt_array(opt, json_child, element->au32Depth, sizeof(int32_t) * 1 * VPSS_MAX_PHY_CHN_NUM, array_size_list, jsonb_opt_int32_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enSnsSize");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enSnsSize", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enSnsSize, sizeof(uint32_t));
}
}
void jsonb_opt_sdk_venc_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_venc_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "VencChn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "VencChn", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->VencChn, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "VpssGrp");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "VpssGrp", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->VpssGrp, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "VpssChn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "VpssChn", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->VpssChn, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "u32Profile");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "u32Profile", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->u32Profile, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enPayLoad");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enPayLoad", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enPayLoad, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enSize");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enSize", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enSize, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "bRcnRefShareBuf");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "bRcnRefShareBuf", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->bRcnRefShareBuf, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enRcMode");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enRcMode", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enRcMode, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enGopMode");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enGopMode", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enGopMode, sizeof(uint32_t));
}
}
void jsonb_opt_sdk_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_cfg_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "s32SysNum");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "s32SysNum", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->s32SysNum, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "as32SysId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "as32SysId", json_child);
    }
    const size_t array_size_list[] = {1,0};
    jsonb_opt_array(opt, json_child, element->as32SysId, sizeof(int32_t) * 1 * 1, array_size_list, jsonb_opt_int32_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "astSysInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "astSysInfo", json_child);
    }
    const size_t array_size_list[] = {1,0};
    jsonb_opt_array(opt, json_child, element->astSysInfo, sizeof(sdk_sys_info_t) * 1 * 1, array_size_list, jsonb_opt_sdk_sys_info_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "s32ViNum");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "s32ViNum", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->s32ViNum, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "as32ViId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "as32ViId", json_child);
    }
    const size_t array_size_list[] = {VI_MAX_DEV_NUM,0};
    jsonb_opt_array(opt, json_child, element->as32ViId, sizeof(int32_t) * 1 * VI_MAX_DEV_NUM, array_size_list, jsonb_opt_int32_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "astViInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "astViInfo", json_child);
    }
    const size_t array_size_list[] = {VI_MAX_DEV_NUM,0};
    jsonb_opt_array(opt, json_child, element->astViInfo, sizeof(sdk_vi_info_t) * 1 * VI_MAX_DEV_NUM, array_size_list, jsonb_opt_sdk_vi_info_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "s32VpssNum");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "s32VpssNum", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->s32VpssNum, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "as32VpssId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "as32VpssId", json_child);
    }
    const size_t array_size_list[] = {1,0};
    jsonb_opt_array(opt, json_child, element->as32VpssId, sizeof(int32_t) * 1 * 1, array_size_list, jsonb_opt_int32_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "astVpssInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "astVpssInfo", json_child);
    }
    const size_t array_size_list[] = {1,0};
    jsonb_opt_array(opt, json_child, element->astVpssInfo, sizeof(sdk_vpss_info_t) * 1 * 1, array_size_list, jsonb_opt_sdk_vpss_info_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "s32VencNum");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "s32VencNum", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->s32VencNum, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "as32VencId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "as32VencId", json_child);
    }
    const size_t array_size_list[] = {VENC_MAX_CHN_NUM,0};
    jsonb_opt_array(opt, json_child, element->as32VencId, sizeof(int32_t) * 1 * VENC_MAX_CHN_NUM, array_size_list, jsonb_opt_int32_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "astVencInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "astVencInfo", json_child);
    }
    const size_t array_size_list[] = {VENC_MAX_CHN_NUM,0};
    jsonb_opt_array(opt, json_child, element->astVencInfo, sizeof(sdk_venc_info_t) * 1 * VENC_MAX_CHN_NUM, array_size_list, jsonb_opt_sdk_venc_info_t);
}
}
