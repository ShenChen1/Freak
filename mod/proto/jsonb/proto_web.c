#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "proto_web.h"
void jsonb_opt_proto_web_svr_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_web_svr_cfg_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "id");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "id", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->id, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "enable");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "enable", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->enable, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "ip");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "ip", json_child);
    }
    jsonb_opt_string(opt, json_child, element->ip, 64);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "port");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "port", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->port, sizeof(int));
}
}
void jsonb_opt_proto_web_svr_cap_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_web_svr_cap_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "id");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "id", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->id, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "dummy");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "dummy", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->dummy, sizeof(int));
}
}
void jsonb_opt_proto_web_svr_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_web_svr_t *element= e;
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
    const size_t array_size_list[] = {WEB_ITEM_MAX,0};
    jsonb_opt_array(opt, json_child, element->caps, sizeof(proto_web_svr_cap_t) * 1 * WEB_ITEM_MAX, array_size_list, jsonb_opt_proto_web_svr_cap_t);
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
    const size_t array_size_list[] = {WEB_ITEM_MAX,0};
    jsonb_opt_array(opt, json_child, element->cfgs, sizeof(proto_web_svr_cfg_t) * 1 * WEB_ITEM_MAX, array_size_list, jsonb_opt_proto_web_svr_cfg_t);
}
}
