#ifndef __MSGBOX_H__
#define __MSGBOX_H__

#include "common.h"

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

int msgbox_init(int size);
int msgbox_deinit();
int msgbox_do_handler(void *in, size_t isize, void *out, size_t *osize);
int msgbox_reg_handler(int key, void *value);

#endif /* __MSGBOX_H__ */