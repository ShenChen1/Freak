#include "inc/cfg.h"
#include "inc/msgbox.h"
#include "vsf/osd_mgr.h"

static int msgbox_osd_set(msgbox_param_t *param)
{
    int ret;
    proto_vsf_osd_cfg_t *in = param->in;

    if (param->format == PROTO_FORMAT_JSON) {
        in = malloc(sizeof(proto_vsf_osd_cfg_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_vsf_osd_cfg_t(JSONB_OPT_J2S, json, in, sizeof(proto_vsf_osd_cfg_t));
            cJSON_Delete(json);
        }
    }

    vsf_osd_mgr_t *obj = vsf_createOsdMgr();
    assert(obj && obj->set);
    ret = obj->set(obj, in);
    if (!ret) {
        cfg_get_member(osd)->cfgs[param->chn] = *in;
    }
    *param->osize = 0;

    if (param->format == PROTO_FORMAT_JSON) {
        free(in);
    }

    return ret;
}

static int msgbox_osd_get(msgbox_param_t *param)
{
    int ret;
    proto_vsf_osd_cfg_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        out = malloc(sizeof(proto_vsf_osd_cfg_t));
    }

    vsf_osd_mgr_t *obj = vsf_createOsdMgr();
    assert(obj && obj->get);
    ret = obj->get(obj, out);
    *param->osize = sizeof(proto_vsf_osd_cfg_t);

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_vsf_osd_cfg_t(JSONB_OPT_S2J, json, out, sizeof(proto_vsf_osd_cfg_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(out);
    }

    return ret;
}

static int msgbox_osd_cap(msgbox_param_t *param)
{
    int ret;
    proto_vsf_osd_cap_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        out = malloc(sizeof(proto_vsf_osd_cap_t));
    }

    vsf_osd_mgr_t *obj = vsf_createOsdMgr();
    assert(obj && obj->cap);
    ret = obj->cap(obj, out);
    *param->osize = sizeof(proto_vsf_osd_cap_t);

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_vsf_osd_cap_t(JSONB_OPT_S2J, json, out, sizeof(proto_vsf_osd_cap_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(out);
    }

    return ret;
}

static int msgbox_osd_num(msgbox_param_t *param)
{
    int ret;
    int *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        out = malloc(sizeof(int));
    }


    vsf_osd_mgr_t *obj = vsf_createOsdMgr();
    assert(obj && obj->num);
    ret = obj->num(obj);
    if (ret >= 0) {
        *out = ret;
        ret = 0;
    } else {
        ret = EINVAL;
    }
    *param->osize = sizeof(int);

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_vsf_osd_cap_t(JSONB_OPT_S2J, json, out, sizeof(int));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(out);
    }

    return ret;
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
    } else {
        assert(0);
    }

    return ret;
}