#include "common.h"
#include "msgbox.h"
#include "proto.h"
#include "inc/cfg.h"
#include "web/svr_mgr.h"

static int msgbox_svr_set(msgbox_param_t *param)
{
    int ret;
    proto_web_svr_cfg_t cfg;

    proto_server_data_pre(param->format, jsonb_opt_proto_web_svr_cfg_t, param->in, param->isize, &cfg, sizeof(proto_web_svr_cfg_t));

    web_svr_mgr_t *obj = web_createSvrMgr();
    assert(obj && obj->set);
    ret = obj->set(obj, &cfg);
    assert(ret == 0);
    cfg_get_member(server)->cfgs[cfg.id] = cfg;

    proto_server_data_post(param->format, jsonb_opt_proto_web_svr_cfg_t, &cfg, sizeof(proto_web_svr_cfg_t), param->out, param->osize);
    return ret;
}

static int msgbox_svr_get(msgbox_param_t *param)
{
    int ret;
    proto_web_svr_cfg_t cfg;

    proto_server_data_pre(param->format, jsonb_opt_proto_web_svr_cfg_t, param->in, param->isize, &cfg, sizeof(proto_web_svr_cfg_t));

    web_svr_mgr_t *obj = web_createSvrMgr();
    assert(obj && obj->get);
    ret = obj->get(obj, &cfg);
    assert(ret == 0);

    proto_server_data_post(param->format, jsonb_opt_proto_web_svr_cfg_t, &cfg, sizeof(proto_web_svr_cfg_t), param->out, param->osize);
    return ret;
}

static int msgbox_svr_cap(msgbox_param_t *param)
{
    int ret;
    proto_web_svr_cap_t cap;

    proto_server_data_pre(param->format, jsonb_opt_proto_web_svr_cap_t, param->in, param->isize, &cap, sizeof(proto_web_svr_cap_t));

    web_svr_mgr_t *obj = web_createSvrMgr();
    assert(obj && obj->cap);
    ret = obj->cap(obj, &cap);
    assert(ret == 0);

    proto_server_data_post(param->format, jsonb_opt_proto_web_svr_cap_t, &cap, sizeof(proto_web_svr_cap_t), param->out, param->osize);
    return ret;
}

static int msgbox_svr_num(msgbox_param_t *param)
{
    proto_num_t num;

    proto_server_data_pre(param->format, jsonb_opt_proto_num_t, param->in, param->isize, &num, sizeof(int));

    web_svr_mgr_t *obj = web_createSvrMgr();
    assert(obj && obj->num);
    num.num = obj->num(obj);
    assert(num.num >= 0);

    proto_server_data_post(param->format, jsonb_opt_proto_num_t, &num, sizeof(int), param->out, param->osize);
    return 0;
}

int msgbox_web_svr(msgbox_param_t *param)
{
    int ret = 0;

    if (param->action == PROTO_ACTION_SET) {
        ret = msgbox_svr_set(param);
    } else if (param->action == PROTO_ACTION_GET) {
        ret = msgbox_svr_get(param);
    } else if (param->action == PROTO_ACTION_CAP) {
        ret = msgbox_svr_cap(param);
    } else if (param->action == PROTO_ACTION_NUM) {
        ret = msgbox_svr_num(param);
    } else {
        assert(0);
    }

    return ret;
}