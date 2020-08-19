#ifndef __PROTO_BSP_JSONB_C_API_H__
#define __PROTO_BSP_JSONB_C_API_H__
#include "jsonb_c_type.h"
extern void jsonb_opt_proto_bsp_dummy_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int value;
} proto_bsp_dummy_t;
extern void jsonb_opt_proto_bsp_board_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    char model[32];
    char sn[32];
    char chip[32];
} proto_bsp_board_t;
#endif /* __PROTO_BSP_JSONB_C_API_H__ */
