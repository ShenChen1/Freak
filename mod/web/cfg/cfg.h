#ifndef __CFG_JSONB_C_API_H__
#define __CFG_JSONB_C_API_H__
#include "jsonb_c_type.h"
#include "proto_web.h"
extern void jsonb_opt_web_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    proto_web_svr_t server;
} web_cfg_t;
#endif /* __CFG_JSONB_C_API_H__ */
