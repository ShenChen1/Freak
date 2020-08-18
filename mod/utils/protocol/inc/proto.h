#ifndef __PROTO_H__
#define __PROTO_H__

#include <stdint.h>

typedef struct {
    uint8_t     version;
    uint8_t     errno;
    uint8_t     chn;
    uint8_t     action;
    uint8_t     key;
    uint16_t    size;
    uint8_t     data[0];
} proto_header_t;

typedef enum {
    PROTP_ACTION_GET,
    PROTP_ACTION_SET,
} proto_action_e;

typedef enum {
    PROTP_BSP_KEY_TIME,
    PROTP_BSP_KEY_ETH,
    PROTP_BSP_KEY_MAX,
} proto_bsp_key_e;

#endif /* __PROTO_H__ */