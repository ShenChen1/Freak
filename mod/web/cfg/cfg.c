#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "cfg.h"
void jsonb_opt_web_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    web_cfg_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "server");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "server", json_child);
    }
    jsonb_opt_proto_web_svr_t(opt, json_child, &element->server, sizeof(proto_web_svr_t));
}
}
