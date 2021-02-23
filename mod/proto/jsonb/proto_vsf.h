#ifndef __PROTO_VSF_JSONB_C_API_H__
#define __PROTO_VSF_JSONB_C_API_H__
#include "jsonb_c_type.h"
#include "proto_base.h"
#define VSF_CHN_MAX (2)
#define VSF_FRAME_MAX (8)
#define VSF_STREAM_MAX (6)
#define VSF_OSD_MAX (16)
extern void jsonb_opt_proto_vsf_chn_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int rotate;
    bool freeze;
    bool mirror;
} proto_vsf_chn_cfg_t;
extern void jsonb_opt_proto_vsf_chn_cap_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int chn;
} proto_vsf_chn_cap_t;
extern void jsonb_opt_proto_vsf_chn_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    proto_num_t num;
    proto_vsf_chn_cap_t caps[VSF_CHN_MAX];
    proto_vsf_chn_cap_t cfgs[VSF_CHN_MAX];
} proto_vsf_chn_t;
extern void jsonb_opt_proto_vsf_frame_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int enable;
    int format;
    int width;
    int height;
    int fps;
} proto_vsf_frame_cfg_t;
extern void jsonb_opt_proto_vsf_frame_cap_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int chn;
    int subchn;
} proto_vsf_frame_cap_t;
extern void jsonb_opt_proto_vsf_frame_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    proto_num_t num;
    proto_vsf_frame_cap_t caps[VSF_FRAME_MAX];
    proto_vsf_frame_cfg_t cfgs[VSF_FRAME_MAX];
} proto_vsf_frame_t;
extern void jsonb_opt_proto_vsf_stream_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int enable;
    int encode;
    int width;
    int height;
    int fps;
} proto_vsf_stream_cfg_t;
extern void jsonb_opt_proto_vsf_stream_cap_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int chn;
    int subchn;
} proto_vsf_stream_cap_t;
extern void jsonb_opt_proto_vsf_stream_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    proto_num_t num;
    proto_vsf_stream_cap_t caps[VSF_STREAM_MAX];
    proto_vsf_stream_cfg_t cfgs[VSF_STREAM_MAX];
} proto_vsf_stream_t;
extern void jsonb_opt_proto_vsf_osd_mask_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    proto_point_t points[4];
    uint32_t color;
} proto_vsf_osd_mask_t;
extern void jsonb_opt_proto_vsf_osd_text_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    proto_point_t point;
    uint32_t size;
    uint32_t color;
    char text[128];
} proto_vsf_osd_text_t;
extern void jsonb_opt_proto_vsf_osd_objs_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint32_t num;
    proto_rect_t rects[32];
} proto_vsf_osd_objs_t;
extern void jsonb_opt_proto_vsf_osd_cfg_union_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    char condition[32];
    union {
        proto_vsf_osd_mask_t mask;
        proto_vsf_osd_text_t text;
        proto_vsf_osd_objs_t objs;
    };
} proto_vsf_osd_cfg_union_t;
extern void jsonb_opt_proto_vsf_osd_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int enable;
    proto_vsf_osd_cfg_union_t info;
} proto_vsf_osd_cfg_t;
extern void jsonb_opt_proto_vsf_osd_cap_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int chn;
    int subchn;
} proto_vsf_osd_cap_t;
extern void jsonb_opt_proto_vsf_osd_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    proto_num_t num;
    proto_vsf_osd_cap_t caps[VSF_OSD_MAX];
    proto_vsf_osd_cfg_t cfgs[VSF_OSD_MAX];
} proto_vsf_osd_t;
#endif /* __PROTO_VSF_JSONB_C_API_H__ */
