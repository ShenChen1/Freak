#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "sdk_cfg.h"
void jsonb_opt_sdk_sys_vb_pool_info_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_sys_vb_pool_info_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "stVbConf");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "stVbConf", json_child);
    }
    jsonb_opt_sdk_sys_vb_info_t(opt, json_child, &element->stVbConf, sizeof(sdk_sys_vb_info_t));
}
}
void jsonb_opt_sdk_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_cfg_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
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
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "astSysInfo");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "astSysInfo", json_child);
    }
    const size_t array_size_list[] = {1,0};
    jsonb_opt_array(opt, json_child, element->astSysInfo, sizeof(sdk_sys_info_t) * 1 * 1, array_size_list, jsonb_opt_sdk_sys_info_t);
}
}
