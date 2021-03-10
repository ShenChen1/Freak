#ifndef __PROTO_HDR_JSONB_C_API_H__
#define __PROTO_HDR_JSONB_C_API_H__
#include "jsonb_c_type.h"
#define PROTO_PACKAGE_MAXSIZE (2048)
extern void jsonb_opt_proto_action_e(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef enum {
    PROTO_ACTION_NUM,
    PROTO_ACTION_CAP,
    PROTO_ACTION_GET,
    PROTO_ACTION_SET,
    PROTO_ACTION_TGR,
} proto_action_e;
extern void jsonb_opt_proto_format_e(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef enum {
    PROTO_FORMAT_STRUCTE,
    PROTO_FORMAT_JSON,
} proto_format_e;
extern void jsonb_opt_proto_key_e(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef enum {
    PROTO_BSP_KEY_DUMMY,
    PROTO_VSF_KEY_FRAME,
    PROTO_VSF_KEY_STREAM,
    PROTO_VSF_KEY_OSD,
    PROTO_RTSP_KEY_SVR,
    PROTO_WEB_KEY_SVR,
    PROTO_APP_KEY_ALG,
    PROTO_KEY_MAX,
} proto_key_e;
extern void jsonb_opt_proto_header_t(jsonb_opt_e opt, cJSON *json, void *element, size_t size);
typedef struct {
    uint8_t version;
    uint64_t ts;
    uint8_t chn;
    proto_key_e key;
    proto_action_e action;
    uint8_t errcode;
    proto_format_e format;
    uint16_t size;
    uint8_t data[0];
} proto_header_t;
#endif /* __PROTO_HDR_JSONB_C_API_H__ */
