#ifndef __MSG_H__
#define __MSG_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t     version;
    uint8_t     action;
    uint64_t    timestamp;
    uint32_t    category;
    uint32_t    key;
    uint32_t    size;
    uint8_t     data[0];
} msg_header_t;

#endif /* __MSG_H__ */