#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "proto_app.h"
void jsonb_opt_proto_app_alg_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_app_alg_cfg_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "type");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "type", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->type, sizeof(int));
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
        json_child = cJSON_GetObjectItem(json, "algpath");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "algpath", json_child);
    }
    jsonb_opt_string(opt, json_child, element->algpath, 64);
}
}
void jsonb_opt_proto_app_alg_cap_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_app_alg_cap_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "frame");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "frame", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->frame, sizeof(int));
}
}
void jsonb_opt_proto_app_alg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_app_alg_t *element= e;
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
    const size_t size_list_data[] = {APP_ITEM_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->caps, sizeof(proto_app_alg_cap_t) * 1 * APP_ITEM_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_app_alg_cap_t);
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
    const size_t size_list_data[] = {APP_ITEM_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->cfgs, sizeof(proto_app_alg_cfg_t) * 1 * APP_ITEM_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_app_alg_cfg_t);
}
}
void jsonb_opt_proto_app_alg_obj_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_app_alg_obj_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "score");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "score", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->score, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "text");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "text", json_child);
    }
    jsonb_opt_string(opt, json_child, element->text, 16);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "rect");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "rect", json_child);
    }
    jsonb_opt_proto_rect_t(opt, json_child, &element->rect, sizeof(proto_rect_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "keypoints");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "keypoints", json_child);
    }
    const size_t size_list_data[] = {ALG_KEY_POINT_NUM,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->keypoints, sizeof(proto_point_t) * 1 * ALG_KEY_POINT_NUM, size_list_data, size_list_len, 0, jsonb_opt_proto_point_t);
}
}
void jsonb_opt_proto_app_alg_result_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_app_alg_result_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "num");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "num", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->num, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "objs");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "objs", json_child);
    }
    const size_t size_list_data[] = {ALG_OBJ_NUM_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->objs, sizeof(proto_app_alg_obj_t) * 1 * ALG_OBJ_NUM_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_app_alg_obj_t);
}
}
