#ifndef __SDK_CFG_JSONB_C_API_H__
#define __SDK_CFG_JSONB_C_API_H__
#include "jsonb_c_type.h"
#include "inc/sdk_common.h"
#include "proto_vsf.h"
extern void jsonb_opt_sdk_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int value;
} sdk_cfg_t;
#endif /* __SDK_CFG_JSONB_C_API_H__ */
