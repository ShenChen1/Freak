#ifndef __PROTO_APP_JSONB_C_API_H__
#define __PROTO_APP_JSONB_C_API_H__
#include "jsonb_c_type.h"
#define APP_ITEM_MAX (3)
extern void jsonb_opt_proto_app_alg_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int type;
    int enable;
    char algpath[64];
} proto_app_alg_cfg_t;
extern void jsonb_opt_proto_app_alg_cap_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int dummy;
} proto_app_alg_cap_t;
extern void jsonb_opt_proto_app_alg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int num;
    proto_app_alg_cap_t caps[APP_ITEM_MAX];
    proto_app_alg_cfg_t cfgs[APP_ITEM_MAX];
} proto_app_alg_t;
#endif /* __PROTO_APP_JSONB_C_API_H__ */