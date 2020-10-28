#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "proto_vsf.h"
void jsonb_opt_pic_size_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    pic_size_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "w");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "w", json_child);
    }
    jsonb_opt_uint16_t(opt, json_child, &element->w, sizeof(uint16_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "h");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "h", json_child);
    }
    jsonb_opt_uint16_t(opt, json_child, &element->h, sizeof(uint16_t));
}
}
void jsonb_opt_proto_vsf_vi_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_vi_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "id");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "id", json_child);
    }
    jsonb_opt_uint16_t(opt, json_child, &element->id, sizeof(uint16_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "enable");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enable", json_child);
    }
    jsonb_opt_bool(opt, json_child, &element->enable, sizeof(bool));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "rez");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "rez", json_child);
    }
    jsonb_opt_pic_size_t(opt, json_child, &element->rez, sizeof(pic_size_t));
}
}
void jsonb_opt_proto_vsf_venc_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_venc_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "value");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "value", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->value, sizeof(int));
}
}
void jsonb_opt_proto_vsf_vpss_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_vpss_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "value");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "value", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->value, sizeof(int));
}
}
