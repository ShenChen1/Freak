#ifndef __PROTO_H__
#define __PROTO_H__

#include <stdint.h>
#include <string.h>
#include "node.h"
#include "path.h"
#include "proto_bsp.h"

#define PROTO_PACKAGE_MAXSIZE   (1024)

typedef struct {
    uint8_t     version;
    uint8_t     chn;
    uint8_t     action;
    uint8_t     key;
    uint16_t    errcode;
    uint16_t    size;
    uint8_t     data[0];
} proto_header_t;

typedef enum {
    PROTO_ACTION_GET,
    PROTO_ACTION_SET,
} proto_action_e;

typedef enum {
    PROTP_BSP_KEY_DUMMY,
    PROTP_BSP_KEY_TIME,
    PROTP_BSP_KEY_ETH,
    PROTP_BSP_KEY_MAX,
} proto_bsp_key_e;

#define proto_header_dump(_package) \
(void)({ \
    proto_header_t *p = (void *)(_package); \
    infof("----------------------"); \
    infof("- version:   %u", p->version); \
    infof("- chn:       %u", p->chn); \
    infof("- action:    %s", p->action ? "SET" : "GET"); \
    infof("- key:       %u", p->key); \
    infof("- errcode:   %u", p->errcode); \
    infof("- size:      %u", p->size); \
    infof("----------------------"); \
})

#define proto_package_fill(_package, _chn, _action, _key, _data, _size) \
(void)({ \
    proto_header_t *p = (void *)(_package); \
    p->version = 1; \
    p->chn = (_chn); \
    p->action = (_action); \
    p->key = (_key); \
    p->errcode = 0; \
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