#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "cfg.h"
void jsonb_opt_bsp_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    bsp_cfg_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "board");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "board", json_child);
    }
    jsonb_opt_proto_bsp_board_t(opt, json_child, &element->board, sizeof(proto_bsp_board_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "dummy");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "dummy", json_child);
    }
    const size_t array_size_list[] = {3,0};
    jsonb_opt_array(opt, json_child, element->dummy, sizeof(proto_bsp_dummy_t) * 1 * 3, array_size_list, jsonb_opt_proto_bsp_dummy_t);
}
}
