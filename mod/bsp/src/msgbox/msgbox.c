#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "box.h"
#include "log.h"
#include "inc/msgbox.h"
#include "inc/cfg.h"

static box_t s_box = NULL;
extern int msgbox_dummy(msgbox_param_t *param);

int msgbox_init()
{
    int ret = 0;

    s_box = box_new(PROTP_BSP_KEY_MAX);
    assert(s_box);

    ret |= box_add(s_box, PROTP_BSP_KEY_DUMMY, msgbox_dummy);
    return ret;
}

int msgbox_deinit()
{
    box_free(s_box);
    return 0;
}

int msgbox_do_handler(void *in, size_t isize, void *out, size_t *osize)
{
    size_t size = 0;
    proto_header_t *packet_in = in;
    proto_header_t *packet_out = out;
    msgbox_handler_t func = NULL;

    proto_header_dump(packet_in);
    func = box_get(s_box, packet_in->key, NULL);
    assert(func);

    msgbox_param_t param = {};
    param.chn = packet_in->chn;
    param.action = packet_in->action;
    param.format = packet_in->format;
    param.in = packet_in->data;
    param.isize = packet_in->size;
    param.out = packet_out->data;
    param.osize = &size;

    proto_package_fill(packet_out, packet_in->chn, packet_in->key, packet_in->action, packet_in->format, packet_in->data, 0);
    packet_out->errcode = func(&param);
    packet_out->size = size;

    proto_header_dump(packet_out);

    if (packet_in->action == PROTO_ACTION_SET) {
        cfg_save(PROTO_BSP_CFG_PATH);
    }

    *osize = packet_out->size + sizeof(proto_header_t);
    return 0;
}

