#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "proto_base.h"
void jsonb_opt_proto_num_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_num_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "num");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "num", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->num, sizeof(int32_t));
}
}
void jsonb_opt_proto_point_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_point_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "x");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "x", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->x, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "y");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "y", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->y, sizeof(int32_t));
}
}
void jsonb_opt_proto_size_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_size_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "width");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "width", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->width, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "height");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "height", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->height, sizeof(uint32_t));
}
}
void jsonb_opt_proto_rect_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_rect_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "x");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "x", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->x, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "y");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "y", json_child);
    }
    jsonb_opt_int32_t(opt, json_child, &element->y, sizeof(int32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "w");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "w", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->w, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "h");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "h", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->h, sizeof(uint32_t));
}
}
