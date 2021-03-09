#include "common.h"
#include "msgbox.h"
#include "proto.h"
#include "bsp/dummy.h"
#include "inc/cfg.h"
#include "jsonb_c_base.h"

static int msgbox_dummy_set(msgbox_param_t *param)
{
    int ret;
    proto_bsp_dummy_cfg_t cfg;

    proto_server_data_pre(param->format, jsonb_opt_proto_bsp_dummy_cfg_t, param->in, param->isize, &cfg, sizeof(proto_bsp_dummy_cfg_t));

    bsp_dummy_t *obj = bsp_createDummy(param->chn);
    assert(obj && obj->set);
    ret = obj->set(obj, &cfg);
    assert(ret == 0);
    cfg_get_member(dummy)->cfgs[param->chn] = cfg;

    proto_server_data_post(param->format, jsonb_opt_proto_bsp_dummy_cfg_t, &cfg, sizeof(proto_bsp_dummy_cfg_t), param->out, param->osize);
    return ret;
}

static int msgbox_dummy_get(msgbox_param_t *param)
{
    int ret;
    proto_bsp_dummy_cfg_t cfg;

    proto_server_data_pre(param->format, jsonb_opt_proto_bsp_dummy_cfg_t, param->in, param->isize, &cfg, sizeof(proto_bsp_dummy_cfg_t));

    bsp_dummy_t *obj = bsp_createDummy(param->chn);
    assert(obj && obj->get);
    ret = obj->get(obj, &cfg);
    assert(ret == 0);

    proto_server_data_post(param->format, jsonb_opt_proto_bsp_dummy_cfg_t, &cfg, sizeof(proto_bsp_dummy_cfg_t), param->out, param->osize);
    return ret;
}

static int msgbox_dummy_cap(msgbox_param_t *param)
{
    int ret;
    proto_bsp_dummy_cap_t cap;

    proto_server_data_pre(param->format, jsonb_opt_proto_bsp_dummy_cap_t, param->in, param->isize, &cap, sizeof(proto_bsp_dummy_cap_t));

    bsp_dummy_t *obj = bsp_createDummy(param->chn);
    assert(obj && obj->cap);
    ret = obj->cap(obj, &cap);
    assert(ret == 0);

    proto_server_data_post(param->format, jsonb_opt_proto_bsp_dummy_cap_t, &cap, sizeof(proto_bsp_dummy_cap_t), param->out, param->osize);
    return ret;
}

static int msgbox_dummy_num(msgbox_param_t *param)
{
    proto_num_t num;

    proto_server_data_pre(param->format, jsonb_opt_proto_num_t, param->in, param->isize, &num, sizeof(int));

    num.num = bsp_getDummyNum();
    assert(num.num >= 0);

    proto_server_data_post(param->format, jsonb_opt_proto_num_t, &num, sizeof(int), param->out, param->osize);
    return 0;
}

int msgbox_dummy(msgbox_param_t *param)
{
    int ret = 0;

    if (param->action == PROTO_ACTION_SET) {
        ret = msgbox_dummy_set(param);
    } else if (param->action == PROTO_ACTION_GET) {
        ret = msgbox_dummy_get(param);
    } else if (param->action == PROTO_ACTION_CAP) {
        ret = msgbox_dummy_cap(param);
    } else if (param->action == PROTO_ACTION_NUM) {
        ret = msgbox_dummy_num(param);
    } else {
        assert(0);
    }

    return ret;
}