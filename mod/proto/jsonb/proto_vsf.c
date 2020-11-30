#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "proto_vsf.h"
void jsonb_opt_proto_vsf_frame_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_frame_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "chn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "chn", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->chn, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "subchn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "subchn", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->subchn, sizeof(int));
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
    jsonb_opt_int(opt, json_child, &element->format, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "width");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "width", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->width, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "height");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "height", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->height, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "fps");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "fps", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->fps, sizeof(int));
}
}
void jsonb_opt_proto_vsf_frame_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_frame_cfg_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "frames");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "frames", json_child);
    }
    const size_t array_size_list[] = {VSF_FRAME_MAX,0};
    jsonb_opt_array(opt, json_child, element->frames, sizeof(proto_vsf_frame_t) * 1 * VSF_FRAME_MAX, array_size_list, jsonb_opt_proto_vsf_frame_t);
}
}
void jsonb_opt_proto_vsf_stream_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_stream_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "chn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "chn", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->chn, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "subchn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "subchn", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->subchn, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "encode");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "encode", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->encode, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "width");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "width", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->width, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "height");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "height", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->height, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "fps");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "fps", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->fps, sizeof(int));
}
}
void jsonb_opt_proto_vsf_stream_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_stream_cfg_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "streams");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "streams", json_child);
    }
    const size_t array_size_list[] = {VSF_STREAM_MAX,0};
    jsonb_opt_array(opt, json_child, element->streams, sizeof(proto_vsf_stream_t) * 1 * VSF_STREAM_MAX, array_size_list, jsonb_opt_proto_vsf_stream_t);
}
}
void jsonb_opt_proto_vsf_osd_mask_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_mask_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "points");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "points", json_child);
    }
    const size_t array_size_list[] = {4,0};
    jsonb_opt_array(opt, json_child, element->points, sizeof(proto_point_t) * 1 * 4, array_size_list, jsonb_opt_proto_point_t);
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "color");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "color", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->color, sizeof(uint32_t));
}
}
void jsonb_opt_proto_vsf_osd_text_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_text_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "points");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "points", json_child);
    }
    jsonb_opt_proto_point_t(opt, json_child, &element->points, sizeof(proto_point_t));
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
    jsonb_opt_string(opt, json_child, element->text, 64);
}
}
void jsonb_opt_proto_vsf_osd_union_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_union_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "condition");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "condition", json_child);
    }
    jsonb_opt_string(opt, json_child, element->condition, 32);
}
if (!strncmp("mask", element->condition, sizeof("mask")))
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
            json_child = cJSON_GetObjectItem(json, "mask");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "mask", json_child);
    }
    jsonb_opt_proto_vsf_osd_mask_t(opt, json_child, &element->mask, sizeof(proto_vsf_osd_mask_t));
}
if (!strncmp("text", element->condition, sizeof("text")))
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
            json_child = cJSON_GetObjectItem(json, "text");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "text", json_child);
    }
    jsonb_opt_proto_vsf_osd_text_t(opt, json_child, &element->text, sizeof(proto_vsf_osd_text_t));
}
}
void jsonb_opt_proto_vsf_osd_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "chn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "chn", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->chn, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "subchn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "subchn", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->subchn, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "info");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "info", json_child);
    }
    jsonb_opt_proto_vsf_osd_union_t(opt, json_child, &element->info, sizeof(proto_vsf_osd_union_t));
}
}
void jsonb_opt_proto_vsf_osd_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_cfg_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "osds");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "osds", json_child);
    }
    const size_t array_size_list[] = {VSF_OSD_MAX,0};
    jsonb_opt_array(opt, json_child, element->osds, sizeof(proto_vsf_osd_t) * 1 * VSF_OSD_MAX, array_size_list, jsonb_opt_proto_vsf_osd_t);
}
}
