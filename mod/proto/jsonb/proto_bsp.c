#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "proto_bsp.h"
void jsonb_opt_proto_bsp_dummy_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_bsp_dummy_cfg_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "value");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "value", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->value, sizeof(int));
}
}
void jsonb_opt_proto_bsp_dummy_cap_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_bsp_dummy_cap_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "value");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "value", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->value, sizeof(int));
}
}
void jsonb_opt_proto_bsp_dummy_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_bsp_dummy_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "num");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "num", json_child);
    }
    jsonb_opt_proto_num_t(opt, json_child, &element->num, sizeof(proto_num_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "caps");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "caps", json_child);
    }
    const size_t size_list_data[] = {BSP_ITEM_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->caps, sizeof(proto_bsp_dummy_cap_t) * 1 * BSP_ITEM_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_bsp_dummy_cap_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "cfgs");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "cfgs", json_child);
    }
    const size_t size_list_data[] = {BSP_ITEM_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->cfgs, sizeof(proto_bsp_dummy_cfg_t) * 1 * BSP_ITEM_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_bsp_dummy_cfg_t);
}
}
