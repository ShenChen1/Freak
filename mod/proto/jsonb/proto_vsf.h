#ifndef __PROTO_VSF_JSONB_C_API_H__
#define __PROTO_VSF_JSONB_C_API_H__
#include "jsonb_c_type.h"
extern void jsonb_opt_pic_size_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint16_t w;
    uint16_t h;
} pic_size_t;
extern void jsonb_opt_proto_vsf_vi_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint16_t id;
    bool enable;
    pic_size_t rez;
} proto_vsf_vi_t;
extern void jsonb_opt_proto_vsf_venc_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int value;
} proto_vsf_venc_t;
extern void jsonb_opt_proto_vsf_vpss_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int value;
} proto_vsf_vpss_t;
#endif /* __PROTO_VSF_JSONB_C_API_H__ */
