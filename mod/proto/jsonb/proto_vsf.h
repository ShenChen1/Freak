#ifndef __PROTO_VSF_JSONB_C_API_H__
#define __PROTO_VSF_JSONB_C_API_H__
#include "jsonb_c_type.h"
extern void jsonb_opt_rez_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint16_t height;
    uint16_t width;
    uint16_t fps;
} rez_t;
extern void jsonb_opt_proto_vsf_venc_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint16_t id;
    bool enable;
    rez_t rez;
} proto_vsf_venc_t;
extern void jsonb_opt_proto_vsf_vpss_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int value;
} proto_vsf_vpss_t;
extern void jsonb_opt_proto_vsf_vi_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint16_t id;
    bool enable;
    rez_t rez;
} proto_vsf_vi_t;
extern void jsonb_opt_proto_vsf_sensor_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint16_t id;
    bool enable;
    char sensor[32];
    uint16_t lane;
    uint16_t wdr;
    rez_t rez;
} proto_vsf_sensor_t;
#endif /* __PROTO_VSF_JSONB_C_API_H__ */
