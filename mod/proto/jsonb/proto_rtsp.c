#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "proto_rtsp.h"
void jsonb_opt_proto_rtsp_url_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_rtsp_url_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) assert(0);
        json_child = cJSON_GetObjectItem(json, "url");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "url", json_child);
    }
    jsonb_opt_string(opt, json_child, element->url, 256);
}
}
