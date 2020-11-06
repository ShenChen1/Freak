#ifndef __CFG_JSONB_C_API_H__
#define __CFG_JSONB_C_API_H__
#include "jsonb_c_type.h"
#include "proto_vsf.h"
extern void jsonb_opt_vsf_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    proto_vsf_stream_cfg_t stream;
    proto_vsf_osd_cfg_t osd;
} vsf_cfg_t;
#endif /* __CFG_JSONB_C_API_H__ */
