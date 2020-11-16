#ifndef __PROTO_VSF_JSONB_C_API_H__
#define __PROTO_VSF_JSONB_C_API_H__
#include "jsonb_c_type.h"
#define VSF_FRAME_MAX (8)
#define VSF_STREAM_MAX (6)
#define VSF_OSD_MAX (16)
extern void jsonb_opt_proto_vsf_frame_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int enable;
    int chn;
    int subchn;
    int format;
    int width;
    int height;
    int fps;
} proto_vsf_frame_t;
extern void jsonb_opt_proto_vsf_frame_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int num;
    proto_vsf_frame_t frames[VSF_FRAME_MAX];
} proto_vsf_frame_cfg_t;
extern void jsonb_opt_proto_vsf_stream_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int enable;
    int chn;
    int subchn;
    int encode;
    int width;
    int height;
    int fps;
} proto_vsf_stream_t;
extern void jsonb_opt_proto_vsf_stream_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int num;
    proto_vsf_stream_t streams[VSF_STREAM_MAX];
} proto_vsf_stream_cfg_t;
extern void jsonb_opt_proto_vsf_osd_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int value;
} proto_vsf_osd_t;
extern void jsonb_opt_proto_vsf_osd_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int num;
    proto_vsf_osd_t osds[VSF_OSD_MAX];
} proto_vsf_osd_cfg_t;
#endif /* __PROTO_VSF_JSONB_C_API_H__ */
