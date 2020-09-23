#ifndef __PROTO_H__
#define __PROTO_H__

#include <stdint.h>
#include <string.h>
#include <time.h>
#include "node.h"
#include "path.h"
#include "proto_bsp.h"
#include "proto_rtsp.h"

#define PROTO_PACKAGE_MAXSIZE   (1024)

typedef struct {
    uint8_t     version;
    uint8_t     res[7];
    uint64_t    ts;
    uint8_t     chn;
    uint8_t     key;
    uint8_t     action;
    uint8_t     res1[5];
    uint8_t     errcode;
    uint8_t     format;
    uint8_t     res2[4];
    uint16_t    size;
    uint8_t     data[0];
} proto_header_t;

typedef enum {
    PROTO_ACTION_GET,
    PROTO_ACTION_SET,
} proto_action_e;

typedef enum {
    PROTO_FORMAT_STRUCTE,
    PROTO_FORMAT_JSON,
} proto_format_e;

typedef enum {
    PROTP_BSP_KEY_DUMMY,
    PROTP_BSP_KEY_MAX,
} proto_bsp_key_e;

typedef enum {
    PROTP_VSF_KEY_VI,
    PROTP_VSF_KEY_VPSS,
    PROTP_VSF_KEY_VENC,
    PROTP_VSF_KEY_MAX,
} proto_vsf_key_e;

typedef enum {
    PROTP_RTSP_KEY_OPEN,
    PROTP_RTSP_KEY_CLOSE,
    PROTP_RTSP_KEY_MAX,
} proto_rtsp_key_e;

#define proto_header_dump(_package) \
(void)({ \
    proto_header_t *p = (void *)(_package); \
    infof("----------------------"); \
    infof("- version:   %u", p->version); \
    infof("- ts:        %llu", p->ts); \
    infof("- chn:       %u", p->chn); \
    infof("- key:       %u", p->key); \
    infof("- action:    %s", p->action ? "SET" : "GET"); \
    infof("- errcode:   %u", p->errcode); \
    infof("- format:    %s", p->format ? "JSON" : "STRUCTE"); \
    infof("- size:      %u", p->size); \
    infof("----------------------"); \
})

#define proto_package_fill(_package, _chn, _key, _action, _format, _data, _size) \
(void)({ \
    proto_header_t *p = (void *)(_package); \
  	struct timespec _ts; \
    clock_gettime(CLOCK_MONOTONIC, &_ts);\
    p->version = 1; \
    p->format = 1; \
    p->ts = _ts.tv_sec*1000 + _ts.tv_nsec/1000000; \
    p->chn = (_chn); \
    p->key = (_key); \
    p->action = (_action); \
    p->errcode = 0; \
    p->format = (_format); \
    p->size = (_size); \
    memcpy(p->data, (_data), (_size)); \
})

#define proto_package_size(_package) \
({ \
    proto_header_t *p = (void *)(_package); \
    p->size + sizeof(proto_header_t); \
})

#define proto_package_data(_package) \
({ \
    proto_header_t *p = (void *)(_package); \
    p->data; \
})

#endif /* __PROTO_H__ */