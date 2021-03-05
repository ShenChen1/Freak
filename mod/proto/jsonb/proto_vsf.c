#include "jsonb_c_type.h"
#include "jsonb_c_base.h"
#include "proto_vsf.h"
void jsonb_opt_proto_vsf_chn_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_chn_cfg_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "rotate");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "rotate", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->rotate, sizeof(int));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "freeze");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "freeze", json_child);
    }
    jsonb_opt_bool(opt, json_child, &element->freeze, sizeof(bool));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "mirror");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "mirror", json_child);
    }
    jsonb_opt_bool(opt, json_child, &element->mirror, sizeof(bool));
}
}
void jsonb_opt_proto_vsf_chn_cap_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_chn_cap_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "chn");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "chn", json_child);
    }
    jsonb_opt_int(opt, json_child, &element->chn, sizeof(int));
}
}
void jsonb_opt_proto_vsf_chn_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_chn_t *element= e;
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
    const size_t size_list_data[] = {VSF_CHN_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->caps, sizeof(proto_vsf_chn_cap_t) * 1 * VSF_CHN_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_vsf_chn_cap_t);
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
    const size_t size_list_data[] = {VSF_CHN_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->cfgs, sizeof(proto_vsf_chn_cap_t) * 1 * VSF_CHN_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_vsf_chn_cap_t);
}
}
void jsonb_opt_proto_vsf_frame_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_frame_cfg_t *element= e;
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
void jsonb_opt_proto_vsf_frame_cap_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_frame_cap_t *element= e;
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
}
void jsonb_opt_proto_vsf_frame_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_frame_t *element= e;
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
    const size_t size_list_data[] = {VSF_FRAME_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->caps, sizeof(proto_vsf_frame_cap_t) * 1 * VSF_FRAME_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_vsf_frame_cap_t);
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
    const size_t size_list_data[] = {VSF_FRAME_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->cfgs, sizeof(proto_vsf_frame_cfg_t) * 1 * VSF_FRAME_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_vsf_frame_cfg_t);
}
}
void jsonb_opt_proto_vsf_stream_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_stream_cfg_t *element= e;
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
void jsonb_opt_proto_vsf_stream_cap_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_stream_cap_t *element= e;
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
}
void jsonb_opt_proto_vsf_stream_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_stream_t *element= e;
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
    const size_t size_list_data[] = {VSF_STREAM_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->caps, sizeof(proto_vsf_stream_cap_t) * 1 * VSF_STREAM_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_vsf_stream_cap_t);
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
    const size_t size_list_data[] = {VSF_STREAM_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->cfgs, sizeof(proto_vsf_stream_cfg_t) * 1 * VSF_STREAM_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_vsf_stream_cfg_t);
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
    const size_t size_list_data[] = {4,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->points, sizeof(proto_point_t) * 1 * 4, size_list_data, size_list_len, 0, jsonb_opt_proto_point_t);
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
        json_child = cJSON_GetObjectItem(json, "point");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "point", json_child);
    }
    jsonb_opt_proto_point_t(opt, json_child, &element->point, sizeof(proto_point_t));
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
    jsonb_opt_uint32_t(opt, json_child, &element->size, sizeof(uint32_t));
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
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "text");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "text", json_child);
    }
    jsonb_opt_string(opt, json_child, element->text, 32);
}
}
void jsonb_opt_proto_vsf_osd_texts_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_texts_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "num");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "num", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->num, sizeof(uint32_t));
}
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "texts");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "texts", json_child);
    }
    const size_t size_list_data[] = {VSF_OSD_TEXTS_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->texts, sizeof(proto_vsf_osd_text_t) * 1 * VSF_OSD_TEXTS_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_vsf_osd_text_t);
}
}
void jsonb_opt_proto_vsf_osd_obj_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_obj_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "color");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "color", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->color, sizeof(uint32_t));
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
}
void jsonb_opt_proto_vsf_osd_objs_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_objs_t *element= e;
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
        json_child = cJSON_GetObjectItem(json, "num");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "num", json_child);
    }
    jsonb_opt_uint32_t(opt, json_child, &element->num, sizeof(uint32_t));
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
    const size_t size_list_data[] = {VSF_OSD_OBJS_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->objs, sizeof(proto_vsf_osd_obj_t) * 1 * VSF_OSD_OBJS_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_vsf_osd_obj_t);
}
}
void jsonb_opt_proto_vsf_osd_cfg_union_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_cfg_union_t *element= e;
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
if (!strncmp("texts", element->condition, sizeof("texts")))
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
            json_child = cJSON_GetObjectItem(json, "texts");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "texts", json_child);
    }
    jsonb_opt_proto_vsf_osd_texts_t(opt, json_child, &element->texts, sizeof(proto_vsf_osd_texts_t));
}
if (!strncmp("objs", element->condition, sizeof("objs")))
{
    cJSON *json_child = NULL;
    if (opt == JSONB_OPT_J2S) {
        if (cJSON_IsNull(json)) return;
            json_child = cJSON_GetObjectItem(json, "objs");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "objs", json_child);
    }
    jsonb_opt_proto_vsf_osd_objs_t(opt, json_child, &element->objs, sizeof(proto_vsf_osd_objs_t));
}
}
void jsonb_opt_proto_vsf_osd_cfg_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_cfg_t *element= e;
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
        json_child = cJSON_GetObjectItem(json, "info");
    } else if (opt == JSONB_OPT_S2J) {
        json_child = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "info", json_child);
    }
    jsonb_opt_proto_vsf_osd_cfg_union_t(opt, json_child, &element->info, sizeof(proto_vsf_osd_cfg_union_t));
}
}
void jsonb_opt_proto_vsf_osd_cap_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_cap_t *element= e;
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
}
void jsonb_opt_proto_vsf_osd_t(jsonb_opt_e opt, cJSON *json, void *e, size_t size)
{
    proto_vsf_osd_t *element= e;
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
    const size_t size_list_data[] = {VSF_OSD_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->caps, sizeof(proto_vsf_osd_cap_t) * 1 * VSF_OSD_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_vsf_osd_cap_t);
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
    const size_t size_list_data[] = {VSF_OSD_MAX,0};
    const int size_list_len = sizeof(size_list_data)/sizeof(size_t);
    jsonb_opt_array(opt, json_child, element->cfgs, sizeof(proto_vsf_osd_cfg_t) * 1 * VSF_OSD_MAX, size_list_data, size_list_len, 0, jsonb_opt_proto_vsf_osd_cfg_t);
}
}
