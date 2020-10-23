#ifndef __CFG_JSONB_C_API_H__
#define __CFG_JSONB_C_API_H__
#include "jsonb_c_type.h"
#include "proto_vsf.h"
#define VSF_CHN_MAX (5)
extern void jsonb_opt_vsf_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    proto_vsf_vi_t vi[VSF_CHN_MAX];
    proto_vsf_vpss_t vpss[VSF_CHN_MAX];
    proto_vsf_venc_t venc[VSF_CHN_MAX];
} vsf_cfg_t;
#endif /* __CFG_JSONB_C_API_H__ */
