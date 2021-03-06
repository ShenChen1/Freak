#ifndef __CFG_JSONB_C_API_H__
#define __CFG_JSONB_C_API_H__
#include "jsonb_c_type.h"
#include "proto_vsf.h"
extern void jsonb_opt_vsf_font_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    char path[64];
} vsf_font_cfg_t;
extern void jsonb_opt_vsf_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    proto_vsf_frame_t frame;
    proto_vsf_stream_t stream;
    proto_vsf_osd_t osd;
    vsf_font_cfg_t font;
} vsf_cfg_t;
#endif /* __CFG_JSONB_C_API_H__ */
