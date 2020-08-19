#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "proto_bsp.h"
void jsonb_opt_proto_bsp_dummy_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_bsp_dummy_t *element= e;
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
void jsonb_opt_proto_bsp_board_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_bsp_board_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "chip");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "chip", json_child);
    }
    jsonb_opt_string(opt, json_child, element->chip, 32);
}
}
