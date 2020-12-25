#include "inc/cfg.h"
#include "inc/msgbox.h"
#include "rtsp/svr_mgr.h"

static int msgbox_server_set(msgbox_param_t *param)
{
    int ret;
    proto_rtsp_svr_cfg_t *in = param->in;

    if (param->format == PROTO_FORMAT_JSON) {
        in = malloc(sizeof(proto_rtsp_svr_cfg_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_rtsp_svr_cfg_t(JSONB_OPT_J2S, json, in, sizeof(proto_rtsp_svr_cfg_t));
            cJSON_Delete(json);
        }
    }

    rtsp_svr_mgr_t *obj = rtsp_createSvrMgr();
    assert(obj && obj->set);
    ret = obj->set(obj, in);
    if (!ret) {
        cfg_get_member(server)->cfgs[param->chn] = *in;
    }
    *param->osize = 0;

    if (param->format == PROTO_FORMAT_JSON) {
        free(in);
    }

    return ret;
}

static int msgbox_server_get(msgbox_param_t *param)
{
    int ret;
    proto_rtsp_svr_cfg_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        out = malloc(sizeof(proto_rtsp_svr_cfg_t));
    }

    rtsp_svr_mgr_t *obj = rtsp_createSvrMgr();
    assert(obj && obj->get);
    ret = obj->get(obj, out);
    *param->osize = sizeof(proto_rtsp_svr_cfg_t);

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_rtsp_svr_cfg_t(JSONB_OPT_S2J, json, out, sizeof(proto_rtsp_svr_cfg_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(out);
    }

    return ret;
}

static int msgbox_server_cap(msgbox_param_t *param)
{
    int ret;
    proto_rtsp_svr_cap_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        out = malloc(sizeof(proto_rtsp_svr_cap_t));
    }

    rtsp_svr_mgr_t *obj = rtsp_createSvrMgr();
    assert(obj && obj->cap);
    ret = obj->cap(obj, out);
    *param->osize = sizeof(proto_rtsp_svr_cap_t);

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_rtsp_svr_cap_t(JSONB_OPT_S2J, json, out, sizeof(proto_rtsp_svr_cap_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(out);
    }

    return ret;
}

static int msgbox_server_num(msgbox_param_t *param)
{
    int ret;
    int *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        out = malloc(sizeof(int));
    }

    rtsp_svr_mgr_t *obj = rtsp_createSvrMgr();
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
            jsonb_opt_proto_rtsp_svr_cap_t(JSONB_OPT_S2J, json, out, sizeof(int));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(out);
    }

    return ret;
}

int msgbox_rtsp_server(msgbox_param_t *param)
{
    int ret = 0;

    if (param->action == PROTO_ACTION_SET) {
        ret = msgbox_server_set(param);
    } else if (param->action == PROTO_ACTION_GET) {
        ret = msgbox_server_get(param);
    } else if (param->action == PROTO_ACTION_CAP) {
        ret = msgbox_server_cap(param);
    } else if (param->action == PROTO_ACTION_NUM) {
        ret = msgbox_server_num(param);
    } else {
        assert(0);
    }

    return ret;
}