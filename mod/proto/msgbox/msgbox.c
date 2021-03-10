#include "proto.h"
#include "msgbox.h"
#include "nnm.h"
#include "log.h"

typedef struct {
    const int key;
    const nnm_t node;
    msgbox_handler_t handler;
} msgbox_node_t;

static msgbox_node_t s_msgbox_node[PROTO_KEY_MAX] = {
    {PROTO_BSP_KEY_DUMMY, PROTO_BSP_COM_NODE, NULL},
    {PROTO_RTSP_KEY_SVR, PROTO_RTSP_COM_NODE, NULL},
    {PROTO_VSF_KEY_FRAME, PROTO_VSF_COM_NODE, NULL},
    {PROTO_VSF_KEY_STREAM, PROTO_VSF_COM_NODE, NULL},
    {PROTO_VSF_KEY_OSD, PROTO_VSF_COM_NODE, NULL},
    {PROTO_WEB_KEY_SVR, PROTO_WEB_COM_NODE, NULL},
    {PROTO_APP_KEY_ALG, PROTO_APP_COM_NODE, NULL},
};

int msgbox_do_forward(void *in, size_t isize, void *out, size_t *osize)
{
    int ret;
    nnm_t req = NULL;
    proto_header_t *packet_in  = in;
    proto_header_t *packet_out = out;

    ret = nnm_req_create(s_msgbox_node[packet_in->key].node, &req);
    if (ret < 0) {
        return ret;
    }

    proto_header_dump(packet_in);
    nnm_req_exchange(req, packet_in, proto_package_size(packet_in), (void **)&packet_out, osize);
    proto_header_dump(packet_out);

    assert(packet_out->key == packet_in->key);
    assert(!packet_out->errcode);

    memcpy(out, packet_out, *osize);
    nnm_free(packet_out);
    nnm_req_destory(req);

    return 0;
}

int msgbox_do_handler(void *in, size_t isize, void *out, size_t *osize)
{
    size_t size                = 0;
    proto_header_t *packet_in  = in;
    proto_header_t *packet_out = out;
    msgbox_handler_t func      = NULL;

    proto_header_dump(packet_in);
    func = s_msgbox_node[packet_in->key].handler;
    assert(func);

    msgbox_param_t param = {};
    param.chn            = packet_in->chn;
    param.action         = packet_in->action;
    param.format         = packet_in->format;
    param.in             = packet_in->data;
    param.isize          = packet_in->size;
    param.out            = packet_out->data;
    param.osize          = &size;

    proto_package_fill(packet_out, packet_in->chn, packet_in->key, packet_in->action, packet_in->format, packet_in->data, 0);
    packet_out->errcode = func(&param);
    packet_out->size    = size;

    proto_header_dump(packet_out);

    *osize = packet_out->size + sizeof(proto_header_t);
    return 0;
}

int msgbox_reg_handler(int key, msgbox_handler_t value)
{
    if (key < 0 || key >= PROTO_KEY_MAX) {
        return -1;
    }

    s_msgbox_node[key].handler = value;
    return 0;
}
