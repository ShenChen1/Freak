#ifndef __PROTO_RTSP_JSONB_C_API_H__
#define __PROTO_RTSP_JSONB_C_API_H__
#include "jsonb_c_type.h"
extern void jsonb_opt_proto_rtsp_server_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    char url[32];
    int port;
} proto_rtsp_server_t;
#endif /* __PROTO_RTSP_JSONB_C_API_H__ */
