#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "sdk_cfg.h"
void jsonb_opt_sdk_vi_chn_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_vi_chn_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "pcVideoNode");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "pcVideoNode", json_child);
    }
    const size_t size_list_data[] = {16,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->pcVideoNode, sizeof(char) * 1 * 16, size_list_data, size_list_len, 0, jsonb_opt_char);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "u32Width");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "u32Width", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->u32Width, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "u32Height");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "u32Height", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->u32Height, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "u32BufCnt");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "u32BufCnt", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->u32BufCnt, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "enBufType");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enBufType", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enBufType, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "enWorkMode");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enWorkMode", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enWorkMode, sizeof(uint32_t));
}
}
void jsonb_opt_sdk_vi_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_vi_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "pipeId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "pipeId", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->pipeId, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "chnId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "chnId", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->chnId, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "DevId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "DevId", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->DevId, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "RgaId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "RgaId", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->RgaId, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "imgType");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "imgType", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->imgType, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "u32Width");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "u32Width", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->u32Width, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "u32Height");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "u32Height", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->u32Height, sizeof(int32_t));
}
}
void jsonb_opt_sdk_venc_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_venc_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "DevId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "DevId", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->DevId, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "ViChnId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "ViChnId", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->ViChnId, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "ChnId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "ChnId", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->ChnId, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "enType");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enType", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->enType, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "PicWidth");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "PicWidth", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->PicWidth, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "PicHeight");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "PicHeight", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->PicHeight, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "Gop");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "Gop", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->Gop, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "SrcFrameRateDen");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "SrcFrameRateDen", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->SrcFrameRateDen, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "SrcFrameRateNum");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "SrcFrameRateNum", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->SrcFrameRateNum, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "DstFrameRateDen");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "DstFrameRateDen", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->DstFrameRateDen, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "DstFrameRateNum");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "DstFrameRateNum", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->DstFrameRateNum, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "imageType");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "imageType", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->imageType, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "Profile");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "Profile", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->Profile, sizeof(uint32_t));
}
}
void jsonb_opt_sdk_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_cfg_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "as32ViId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "as32ViId", json_child);
    }
    const size_t size_list_data[] = {4,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->as32ViId, sizeof(int32_t) * 1 * 4, size_list_data, size_list_len, 0, jsonb_opt_int32_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "astViInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "astViInfo", json_child);
    }
    const size_t size_list_data[] = {4,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->astViInfo, sizeof(sdk_vi_info_t) * 1 * 4, size_list_data, size_list_len, 0, jsonb_opt_sdk_vi_info_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "as32VpssId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "as32VpssId", json_child);
    }
    const size_t size_list_data[] = {4,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->as32VpssId, sizeof(int32_t) * 1 * 4, size_list_data, size_list_len, 0, jsonb_opt_int32_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "astVpssInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "astVpssInfo", json_child);
    }
    const size_t size_list_data[] = {4,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->astVpssInfo, sizeof(sdk_vpss_info_t) * 1 * 4, size_list_data, size_list_len, 0, jsonb_opt_sdk_vpss_info_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "as32VencId");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "as32VencId", json_child);
    }
    const size_t size_list_data[] = {4,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->as32VencId, sizeof(int32_t) * 1 * 4, size_list_data, size_list_len, 0, jsonb_opt_int32_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "astVencInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "astVencInfo", json_child);
    }
    const size_t size_list_data[] = {4,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->astVencInfo, sizeof(sdk_venc_info_t) * 1 * 4, size_list_data, size_list_len, 0, jsonb_opt_sdk_venc_info_t);
}
}
