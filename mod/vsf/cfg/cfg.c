#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "cfg.h"
void jsonb_opt_vsf_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    vsf_cfg_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "vi");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "vi", json_child);
    }
    const size_t array_size_list[] = {VSF_CHN_MAX,0};
    jsonb_opt_array(opt, json_child, element->vi, sizeof(proto_vsf_vi_t) * 1 * VSF_CHN_MAX, array_size_list, jsonb_opt_proto_vsf_vi_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "vpss");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "vpss", json_child);
    }
    const size_t array_size_list[] = {VSF_CHN_MAX,0};
    jsonb_opt_array(opt, json_child, element->vpss, sizeof(proto_vsf_vpss_t) * 1 * VSF_CHN_MAX, array_size_list, jsonb_opt_proto_vsf_vpss_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "venc");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "venc", json_child);
    }
    const size_t array_size_list[] = {VSF_CHN_MAX,0};
    jsonb_opt_array(opt, json_child, element->venc, sizeof(proto_vsf_venc_t) * 1 * VSF_CHN_MAX, array_size_list, jsonb_opt_proto_vsf_venc_t);
}
}
