#include "common.h"
#include "log.h"
#include "inc/msgbox.h"
#include "inc/cfg.h"

extern int msgbox_rtsp_open(msgbox_param_t *param);
extern int msgbox_rtsp_close(msgbox_param_t *param);
static msgbox_handler_t s_msgbox[PROTP_RTSP_KEY_MAX] = {
    [PROTP_RTSP_KEY_OPEN] = msgbox_rtsp_open,
    [PROTP_RTSP_KEY_CLOSE] = msgbox_rtsp_close,
};

int msgbox_init()
{
    signal(SIGPIPE, SIG_IGN);
    return 0;
}

int msgbox_deinit()
{
    return 0;
}

int msgbox_do_handler(void *in, size_t isize, void *out, size_t *osize)
{
    size_t size = 0;
    proto_header_t *packet_in = in;
    proto_header_t *packet_out = out;
    msgbox_handler_t func = NULL;

    proto_header_dump(packet_in);
    func = s_msgbox[packet_in->key];
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
    *osize = packet_out->size + sizeof(proto_header_t);
    return 0;
}

