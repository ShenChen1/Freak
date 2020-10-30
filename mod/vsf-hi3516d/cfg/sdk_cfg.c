#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "sdk_cfg.h"
void jsonb_opt_sdk_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    sdk_cfg_t *element= e;
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
