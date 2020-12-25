#ifndef __PROTO_RTSP_JSONB_C_API_H__
#define __PROTO_RTSP_JSONB_C_API_H__
#include "jsonb_c_type.h"
#define RTSP_ITEM_MAX (1)
extern void jsonb_opt_proto_rtsp_svr_cfg_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int enable;
    char ip[64];
    int port;
} proto_rtsp_svr_cfg_t;
extern void jsonb_opt_proto_rtsp_svr_cap_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int id;
    int dummy;
} proto_rtsp_svr_cap_t;
extern void jsonb_opt_proto_rtsp_svr_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    int num;
    proto_rtsp_svr_cap_t caps[RTSP_ITEM_MAX];
    proto_rtsp_svr_cfg_t cfgs[RTSP_ITEM_MAX];
} proto_rtsp_svr_t;
#endif /* __PROTO_RTSP_JSONB_C_API_H__ */
