#ifndef __PROTO_H__
#define __PROTO_H__

#include <stdint.h>
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

#define proto_header_dump(package) ({ \
    infof("----------------------\n"); \
    infof("- version:   %u\n", (package)->version); \
    infof("- chn:       %u\n", (package)->chn); \
    infof("- action:    %s\n", (package)->action ? "SET" : "GET"); \
    infof("- key:       %u\n", (package)->key); \
    infof("- errcode:   %u\n", (package)->errcode); \
    infof("- size:      %u\n", (package)->size); \
    infof("----------------------\n"); \
})

typedef enum {
    PROTP_ACTION_GET,
    PROTP_ACTION_SET,
} proto_action_e;

typedef enum {
    PROTP_BSP_KEY_DUMMY,
    PROTP_BSP_KEY_TIME,
    PROTP_BSP_KEY_ETH,
    PROTP_BSP_KEY_MAX,
} proto_bsp_key_e;

#endif /* __PROTO_H__ */