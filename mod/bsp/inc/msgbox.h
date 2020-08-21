#ifndef __MSGBOX_H__
#define __MSGBOX_H__

#include "proto.h"

typedef struct {
    uint8_t chn;
    uint8_t action;
    uint8_t format;
    void *in;
    size_t isize;
    void *out;
    size_t *osize;
} msgbox_param_t;

typedef int (*msgbox_handler_t)(msgbox_param_t *param);

int msgbox_init();
int msgbox_deinit();
int msgbox_do_handler(void *in, size_t isize, void *out, size_t *osize);

#endif /* __MSGBOX_H__ */