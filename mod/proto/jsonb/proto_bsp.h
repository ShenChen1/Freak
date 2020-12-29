#ifndef __PROTO_BSP_JSONB_C_API_H__
#define __PROTO_BSP_JSONB_C_API_H__
#include "jsonb_c_type.h"
#include "proto_base.h"
#define BSP_ITEM_MAX (8)
extern void jsonb_opt_proto_bsp_dummy_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int value;
} proto_bsp_dummy_cfg_t;
extern void jsonb_opt_proto_bsp_dummy_cap_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int value;
} proto_bsp_dummy_cap_t;
extern void jsonb_opt_proto_bsp_dummy_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    proto_num_t num;
    proto_bsp_dummy_cap_t caps[BSP_ITEM_MAX];
    proto_bsp_dummy_cfg_t cfgs[BSP_ITEM_MAX];
} proto_bsp_dummy_t;
#endif /* __PROTO_BSP_JSONB_C_API_H__ */
