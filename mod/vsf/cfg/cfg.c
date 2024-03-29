#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "cfg.h"
void jsonb_opt_vsf_font_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    vsf_font_cfg_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "path");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "path", json_child);
    }
    jsonb_opt_string(opt, json_child, element->path, 64);
}
}
void jsonb_opt_vsf_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    vsf_cfg_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "frame");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "frame", json_child);
    }
    jsonb_opt_proto_vsf_frame_t(opt, json_child, &element->frame, sizeof(proto_vsf_frame_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "stream");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "stream", json_child);
    }
    jsonb_opt_proto_vsf_stream_t(opt, json_child, &element->stream, sizeof(proto_vsf_stream_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "osd");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "osd", json_child);
    }
    jsonb_opt_proto_vsf_osd_t(opt, json_child, &element->osd, sizeof(proto_vsf_osd_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "font");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "font", json_child);
    }
    jsonb_opt_vsf_font_cfg_t(opt, json_child, &element->font, sizeof(vsf_font_cfg_t));
}
}
