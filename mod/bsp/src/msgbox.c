#include <stdio.h>
#include <assert.h>
#include "box.h"
#include "inc/msgbox.h"

static box_t s_box = NULL;

int msgbox_init()
{
    s_box = box_new(PROTP_BSP_KEY_MAX);
    return !s_box;
}

int msgbox_deinit()
{
    box_free(s_box);
    return 0;
}

int msgbox_add_handler(uint32_t key, msgbox_handler_t func)
{
    return box_add(s_box, key, (void *)func);
}

int msgbox_do_handler(void *in, size_t isize, void *out, size_t *osize)
{
    proto_header_t *packet = in;
    msgbox_handler_t func = box_get(s_box, packet->key, NULL);

    assert(func);
    packet = out;
    packet->errno = func(in, isize, out, osize);

    return 0;
}
