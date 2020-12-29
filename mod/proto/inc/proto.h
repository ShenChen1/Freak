#ifndef __PROTO_H__
#define __PROTO_H__

#include "common.h"
#include "media.h"
#include "node.h"
#include "path.h"

#define PROTO_PACKAGE_MAXSIZE (1024)

typedef struct {
    uint8_t version;
    uint8_t res[7];
    uint64_t ts;
    uint8_t chn;
    uint8_t key;
    uint8_t action;
    uint8_t res1[5];
    uint8_t errcode;
    uint8_t format;
    uint8_t res2[4];
    uint16_t size;
    uint8_t data[0];
} proto_header_t;

typedef enum {
    PROTO_ACTION_NUM,
    PROTO_ACTION_CAP,
    PROTO_ACTION_GET,
    PROTO_ACTION_SET,
} proto_action_e;

typedef enum {
    PROTO_FORMAT_STRUCTE,
    PROTO_FORMAT_JSON,
} proto_format_e;

typedef enum {
    PROTO_BSP_KEY_DUMMY,
    PROTO_BSP_KEY_MAX,
} proto_bsp_key_e;

typedef enum {
    PROTO_VSF_KEY_FRAME,
    PROTO_VSF_KEY_STREAM,
    PROTO_VSF_KEY_OSD,
    PROTO_VSF_KEY_MAX,
} proto_vsf_key_e;

typedef enum {
    PROTO_RTSP_KEY_SERVER,
    PROTO_RTSP_KEY_MAX,
} proto_rtsp_key_e;

typedef enum {
    PROTO_WEB_KEY_SERVER,
    PROTO_WEB_KEY_MAX,
} proto_web_key_e;

typedef enum {
    PROTO_APP_KEY_ALG,
    PROTO_APP_KEY_MAX,
} proto_app_key_e;
#define proto_header_dump(_package)                               \
    (void)({                                                      \
        proto_header_t *p = (void *)(_package);                   \
        infof("----------------------");                          \
        infof("- version:   %u", p->version);                     \
        infof("- ts:        %llu", p->ts);                        \
        infof("- chn:       %u", p->chn);                         \
        infof("- key:       %u", p->key);                         \
        if (p->action == PROTO_ACTION_SET)                        \
        infof("- action:    %s", "SET");                          \
        if (p->action == PROTO_ACTION_GET)                        \
        infof("- action:    %s", "GET");                          \
        if (p->action == PROTO_ACTION_CAP)                        \
        infof("- action:    %s", "CAP");                          \
        if (p->action == PROTO_ACTION_NUM)                        \
        infof("- action:    %s", "NUM");                          \
        infof("- errcode:   %u", p->errcode);                     \
        infof("- format:    %s", p->format ? "JSON" : "STRUCTE"); \
        infof("- size:      %u", p->size);                        \
        infof("----------------------");                          \
    })

#define proto_package_fill(_package, _chn, _key, _action, _format, _data, _size) \
    (void)({                                                                     \
        proto_header_t *p = (void *)(_package);                                  \
        struct timespec _ts;                                                     \
        clock_gettime(CLOCK_MONOTONIC, &_ts);                                    \
        p->version = 1;                                                          \
        p->format  = 1;                                                          \
        p->ts      = _ts.tv_sec * 1000 + _ts.tv_nsec / 1000000;                  \
        p->chn     = (_chn);                                                     \
        p->key     = (_key);                                                     \
        p->action  = (_action);                                                  \
        p->errcode = 0;                                                          \
        p->format  = (_format);                                                  \
        p->size    = (_size);                                                    \
        memcpy(p->data, (_data), (_size));                                       \
    })

#define proto_package_fill_header(_package, _chn, _key, _action, _format, _size) \
    (void)({                                                                     \
        proto_header_t *p = (void *)(_package);                                  \
        struct timespec _ts;                                                     \
        clock_gettime(CLOCK_MONOTONIC, &_ts);                                    \
        p->version = 1;                                                          \
        p->format  = 1;                                                          \
        p->ts      = _ts.tv_sec * 1000 + _ts.tv_nsec / 1000000;                  \
        p->chn     = (_chn);                                                     \
        p->key     = (_key);                                                     \
        p->action  = (_action);                                                  \
        p->errcode = 0;                                                          \
        p->format  = (_format);                                                  \
        p->size    = (_size);                                                    \
    })

#define proto_package_size(_package)            \
    ({                                          \
        proto_header_t *p = (void *)(_package); \
        p->size + sizeof(proto_header_t);       \
    })

#define proto_package_data(_package)            \
    ({                                          \
        proto_header_t *p = (void *)(_package); \
        p->data;                                \
    })

#endif /* __PROTO_H__ */