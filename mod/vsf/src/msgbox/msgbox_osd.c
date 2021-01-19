#include "inc/cfg.h"
#include "inc/msgbox.h"
#include "vsf/osd_mgr.h"

static int msgbox_osd_set(msgbox_param_t *param)
{
    int ret;
    proto_vsf_osd_cfg_t cfg;

    proto_server_data_pre(param->format, jsonb_opt_proto_vsf_osd_cfg_t, param->in, param->isize, &cfg, sizeof(proto_vsf_osd_cfg_t));

    vsf_osd_mgr_t *obj = vsf_createOsdMgr();
    assert(obj && obj->set);
    ret = obj->set(obj, &cfg);
    assert(ret == 0);
    cfg_get_member(osd)->cfgs[cfg.id] = cfg;

    proto_server_data_post(param->format, jsonb_opt_proto_vsf_osd_cfg_t, &cfg, sizeof(proto_vsf_osd_cfg_t), param->out, param->osize);
    return ret;
}

static int msgbox_osd_get(msgbox_param_t *param)
{
    int ret;
    proto_vsf_osd_cfg_t cfg;

    proto_server_data_pre(param->format, jsonb_opt_proto_vsf_osd_cfg_t, param->in, param->isize, &cfg, sizeof(proto_vsf_osd_cfg_t));

    vsf_osd_mgr_t *obj = vsf_createOsdMgr();
    assert(obj && obj->get);
    ret = obj->get(obj, &cfg);
    assert(ret == 0);

    proto_server_data_post(param->format, jsonb_opt_proto_vsf_osd_cfg_t, &cfg, sizeof(proto_vsf_osd_cfg_t), param->out, param->osize);
    return ret;
}

static int msgbox_osd_cap(msgbox_param_t *param)
{
    int ret;
    proto_vsf_osd_cap_t cap;

    proto_server_data_pre(param->format, jsonb_opt_proto_vsf_osd_cap_t, param->in, param->isize, &cap, sizeof(proto_vsf_osd_cap_t));

    vsf_osd_mgr_t *obj = vsf_createOsdMgr();
    assert(obj && obj->cap);
    ret = obj->cap(obj, &cap);
    assert(ret == 0);

    proto_server_data_post(param->format, jsonb_opt_proto_vsf_osd_cap_t, &cap, sizeof(proto_vsf_osd_cap_t), param->out, param->osize);
    return ret;
}

static int msgbox_osd_tgr(msgbox_param_t *param)
{
    int ret;
    proto_vsf_osd_tgr_t tgr;

    proto_server_data_pre(param->format, jsonb_opt_proto_vsf_osd_tgr_t, param->in, param->isize, &tgr, sizeof(proto_vsf_osd_tgr_t));

    vsf_osd_mgr_t *obj = vsf_createOsdMgr();
    assert(obj && obj->tgr);
    ret = obj->tgr(obj, &tgr);
    assert(ret == 0);

    proto_server_data_post(param->format, jsonb_opt_proto_vsf_osd_tgr_t, &tgr, sizeof(proto_vsf_osd_tgr_t), param->out, param->osize);
    return ret;
}

static int msgbox_osd_num(msgbox_param_t *param)
{
    proto_num_t num;

    proto_server_data_pre(param->format, jsonb_opt_proto_num_t, param->in, param->isize, &num, sizeof(int));

    vsf_osd_mgr_t *obj = vsf_createOsdMgr();
    assert(obj && obj->num);
    num.num = obj->num(obj);
    assert(num.num >= 0);

    proto_server_data_post(param->format, jsonb_opt_proto_num_t, &num, sizeof(int), param->out, param->osize);
    return 0;
}

int msgbox_vsf_osd(msgbox_param_t *param)
{
    int ret = 0;

    if (param->action == PROTO_ACTION_SET) {
        ret = msgbox_osd_set(param);
    } else if (param->action == PROTO_ACTION_GET) {
        ret = msgbox_osd_get(param);
    } else if (param->action == PROTO_ACTION_CAP) {
        ret = msgbox_osd_cap(param);
    } else if (param->action == PROTO_ACTION_NUM) {
        ret = msgbox_osd_num(param);
    } else if (param->action == PROTO_ACTION_TGR) {
        ret = msgbox_osd_tgr(param);
    } else {
        assert(0);
    }

    return ret;
}