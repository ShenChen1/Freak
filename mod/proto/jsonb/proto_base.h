#ifndef __PROTO_BASE_JSONB_C_API_H__
#define __PROTO_BASE_JSONB_C_API_H__
#include "jsonb_c_type.h"
extern void jsonb_opt_proto_point_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int32_t x;
    int32_t y;
} proto_point_t;
extern void jsonb_opt_proto_size_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint32_t width;
    uint32_t height;
} proto_size_t;
extern void jsonb_opt_proto_rect_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int32_t x;
    int32_t y;
    uint32_t Width;
    uint32_t height;
} proto_rect_t;
#endif /* __PROTO_BASE_JSONB_C_API_H__ */
