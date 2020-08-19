#ifndef __MSGBOX_H__
#define __MSGBOX_H__

#include "proto.h"

typedef int (*msgbox_handler_t)(uint8_t chn, uint8_t action, void *in, size_t isize, void *out, size_t *osize);

int msgbox_init();
int msgbox_deinit();
int msgbox_add_handler(uint32_t key, msgbox_handler_t func);
int msgbox_do_handler(void *in, size_t isize, void *out, size_t *osize);

#endif /* __MSGBOX_H__ */