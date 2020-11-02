#ifndef __SDK_CFG_JSONB_C_API_H__
#define __SDK_CFG_JSONB_C_API_H__
#include "jsonb_c_type.h"
#include "inc/sdk_common.h"
#include "proto_vsf.h"
extern void jsonb_opt_sdk_sys_vb_pool_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint32_t u32BlkCnt;
    uint32_t u32Width;
    uint32_t u32Height;
} sdk_sys_vb_pool_info_t;
extern void jsonb_opt_sdk_sys_vb_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint32_t u32MaxPoolCnt;
    sdk_sys_vb_pool_info_t astCommPool[VB_MAX_COMM_POOLS];
} sdk_sys_vb_info_t;
extern void jsonb_opt_sdk_sys_info_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    sdk_sys_vb_info_t stVbConf;
} sdk_sys_info_t;
extern void jsonb_opt_sdk_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    sdk_sys_info_t astSysInfo;
} sdk_cfg_t;
#endif /* __SDK_CFG_JSONB_C_API_H__ */
