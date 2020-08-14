#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "cfg.h"
void jsonb_opt_gsf_board_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    gsf_board_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "model");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "model", json_child);
    }
    jsonb_opt_string(opt, json_child, element->model, 32);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "sn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "sn", json_child);
    }
    jsonb_opt_string(opt, json_child, element->sn, 32);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "type");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "type", json_child);
    }
    jsonb_opt_string(opt, json_child, element->type, 32);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "snscnt");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "snscnt", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->snscnt, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "sensor");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "sensor", json_child);
    }
    const size_t array_size_list[] = {8,0};
    jsonb_opt_array(opt, json_child, element->sensor, 32 * array_size_list[0], array_size_list, jsonb_opt_string);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "caps");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "caps", json_child);
    }
    jsonb_opt_uint64_t(opt, json_child, &element->caps, sizeof(uint64_t));
}
}
