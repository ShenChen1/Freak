#include "common.h"
#include "inc/cfg.h"
#include "inc/msgbox.h"
#include "inc/rtsp.h"

static void *s_server = NULL;

int msgbox_rtsp_open(msgbox_param_t *param)
{
    int ret = 0;
    proto_rtsp_server_t *in  = param->in;
    proto_rtsp_server_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        in  = malloc(sizeof(proto_rtsp_server_t));
        out = malloc(sizeof(proto_rtsp_server_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_rtsp_server_t(JSONB_OPT_J2S, json, in, sizeof(proto_rtsp_server_t));
            cJSON_Delete(json);
        }
    }

    if (param->action == PROTO_ACTION_SET) {
        if (s_server == NULL) {
            s_server = rtsp_server_init(in->url, in->port);
        }
        ret = s_server ? 0 : ESRCH;
        *param->osize = 0;
    } else {
        ret = EPERM;
        *param->osize = sizeof(proto_rtsp_server_t);
    }

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_rtsp_server_t(JSONB_OPT_S2J, json, out, sizeof(proto_rtsp_server_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(in);
        free(out);
    }

    return ret;
}

int msgbox_rtsp_close(msgbox_param_t *param)
{
    int ret = 0;
    proto_rtsp_server_t *in  = param->in;
    proto_rtsp_server_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        in  = malloc(sizeof(proto_rtsp_server_t));
        out = malloc(sizeof(proto_rtsp_server_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_rtsp_server_t(JSONB_OPT_J2S, json, in, sizeof(proto_rtsp_server_t));
            cJSON_Delete(json);
        }
    }

    if (param->action == PROTO_ACTION_SET) {
        if (s_server) {
            ret = rtsp_server_uninit(s_server);
        }
        s_server = NULL;
        *param->osize = 0;
    } else {
        ret = EPERM;
        *param->osize = sizeof(proto_rtsp_server_t);
    }

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_rtsp_server_t(JSONB_OPT_S2J, json, out, sizeof(proto_rtsp_server_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(in);
        free(out);
    }

    return ret;
}
