#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "inc/msgbox.h"
#include "inc/cfg.h"

int msgbox_rtsp_open(msgbox_param_t *param)
{
    proto_rtsp_url_t *in = param->in;
    proto_rtsp_url_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        in = malloc(sizeof(proto_rtsp_url_t));
        out = malloc(sizeof(proto_rtsp_url_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_rtsp_url_t(JSONB_OPT_J2S, json, in, sizeof(proto_rtsp_url_t));
            cJSON_Delete(json);
        }
    }

    if (param->action == PROTO_ACTION_SET) {
        *param->osize = 0;
    } else {
        *param->osize = sizeof(proto_rtsp_url_t);
    }

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_rtsp_url_t(JSONB_OPT_S2J, json, out, sizeof(proto_rtsp_url_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(in);
        free(out);
    }

    return 0;
}

int msgbox_rtsp_close(msgbox_param_t *param)
{
    proto_rtsp_url_t *in = param->in;
    proto_rtsp_url_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        in = malloc(sizeof(proto_rtsp_url_t));
        out = malloc(sizeof(proto_rtsp_url_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_rtsp_url_t(JSONB_OPT_J2S, json, in, sizeof(proto_rtsp_url_t));
            cJSON_Delete(json);
        }
    }

    if (param->action == PROTO_ACTION_SET) {
        *param->osize = 0;
    } else {
        *param->osize = sizeof(proto_rtsp_url_t);
    }

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_rtsp_url_t(JSONB_OPT_S2J, json, out, sizeof(proto_rtsp_url_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(in);
        free(out);
    }

    return 0;
}

