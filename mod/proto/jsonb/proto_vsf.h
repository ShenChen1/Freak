#ifndef __PROTO_VSF_JSONB_C_API_H__
#define __PROTO_VSF_JSONB_C_API_H__
#include "jsonb_c_type.h"
extern void jsonb_opt_proto_vsf_venc_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int value;
} proto_vsf_venc_t;
extern void jsonb_opt_proto_vsf_vpss_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int value;
} proto_vsf_vpss_t;
extern void jsonb_opt_proto_vsf_vi_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int value;
} proto_vsf_vi_t;
#endif /* __PROTO_VSF_JSONB_C_API_H__ */
