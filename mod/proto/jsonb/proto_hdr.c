#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "proto_hdr.h"
void jsonb_opt_proto_action_e(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    const josnb_enum_t enummap[] = {
        { PROTO_ACTION_NUM, "PROTO_ACTION_NUM" },
        { PROTO_ACTION_CAP, "PROTO_ACTION_CAP" },
        { PROTO_ACTION_GET, "PROTO_ACTION_GET" },
        { PROTO_ACTION_SET, "PROTO_ACTION_SET" },
        { PROTO_ACTION_TGR, "PROTO_ACTION_TGR" },
    };
    jsonb_opt_enum(opt, json, e, size, enummap, sizeof(enummap)/sizeof(enummap[0]));
}
void jsonb_opt_proto_format_e(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    const josnb_enum_t enummap[] = {
        { PROTO_FORMAT_STRUCTE, "PROTO_FORMAT_STRUCTE" },
        { PROTO_FORMAT_JSON, "PROTO_FORMAT_JSON" },
    };
    jsonb_opt_enum(opt, json, e, size, enummap, sizeof(enummap)/sizeof(enummap[0]));
}
void jsonb_opt_proto_key_e(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    const josnb_enum_t enummap[] = {
        { PROTO_BSP_KEY_DUMMY, "PROTO_BSP_KEY_DUMMY" },
        { PROTO_VSF_KEY_FRAME, "PROTO_VSF_KEY_FRAME" },
        { PROTO_VSF_KEY_STREAM, "PROTO_VSF_KEY_STREAM" },
        { PROTO_VSF_KEY_OSD, "PROTO_VSF_KEY_OSD" },
        { PROTO_RTSP_KEY_SVR, "PROTO_RTSP_KEY_SVR" },
        { PROTO_WEB_KEY_SVR, "PROTO_WEB_KEY_SVR" },
        { PROTO_APP_KEY_ALG, "PROTO_APP_KEY_ALG" },
        { PROTO_KEY_MAX, "PROTO_KEY_MAX" },
    };
    jsonb_opt_enum(opt, json, e, size, enummap, sizeof(enummap)/sizeof(enummap[0]));
}
void jsonb_opt_proto_header_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_header_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "version");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "version", json_child);
    }
    jsonb_opt_uint8_t(opt, json_child, &element->version, sizeof(uint8_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "ts");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "ts", json_child);
    }
    jsonb_opt_uint64_t(opt, json_child, &element->ts, sizeof(uint64_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "chn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "chn", json_child);
    }
    jsonb_opt_uint8_t(opt, json_child, &element->chn, sizeof(uint8_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "key");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "key", json_child);
    }
    jsonb_opt_proto_key_e(opt, json_child, &element->key, sizeof(proto_key_e));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "action");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "action", json_child);
    }
    jsonb_opt_proto_action_e(opt, json_child, &element->action, sizeof(proto_action_e));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "errcode");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "errcode", json_child);
    }
    jsonb_opt_uint8_t(opt, json_child, &element->errcode, sizeof(uint8_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "format");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "format", json_child);
    }
    jsonb_opt_proto_format_e(opt, json_child, &element->format, sizeof(proto_format_e));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "size");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "size", json_child);
    }
    jsonb_opt_uint16_t(opt, json_child, &element->size, sizeof(uint16_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "data");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "data", json_child);
    }
    const size_t size_list_data[] = {0,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->data, sizeof(uint8_t) * 1 * 0, size_list_data, size_list_len, 0, jsonb_opt_uint8_t);
}
}
