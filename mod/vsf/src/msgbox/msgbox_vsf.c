#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "inc/hal/vi.h"
#include "inc/hal/vpss.h"
#include "inc/hal/venc.h"
#include "inc/msgbox.h"
#include "inc/cfg.h"

int msgbox_vsf_vi(msgbox_param_t *param)
{
    int ret = 0;
    vsf_vi_t *obj = NULL;
    proto_vsf_vi_t *in = param->in;
    proto_vsf_vi_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        in = malloc(sizeof(proto_vsf_vi_t));
        out = malloc(sizeof(proto_vsf_vi_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_vsf_vi_t(JSONB_OPT_J2S, json, in, sizeof(proto_vsf_vi_t));
            cJSON_Delete(json);
        }
    }

    ret = VSF_getViNum();
    obj = VSF_createVi(param->chn);
    if (obj == NULL) {
        return -EINVAL;
    }

    if (param->action == PROTO_ACTION_SET) {
        *param->osize = 0;
    } else {
        *param->osize = sizeof(proto_vsf_vi_t);
    }

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_vsf_vi_t(JSONB_OPT_S2J, json, out, sizeof(proto_vsf_vi_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(in);
        free(out);
    }

    return ret;
}

int msgbox_vsf_vpss(msgbox_param_t *param)
{
    proto_vsf_vpss_t *in = param->in;
    proto_vsf_vpss_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        in = malloc(sizeof(proto_vsf_vpss_t));
        out = malloc(sizeof(proto_vsf_vpss_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_vsf_vpss_t(JSONB_OPT_J2S, json, in, sizeof(proto_vsf_vpss_t));
            cJSON_Delete(json);
        }
    }

    if (param->action == PROTO_ACTION_SET) {
        *param->osize = 0;
    } else {
        *param->osize = sizeof(proto_vsf_vpss_t);
    }

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_vsf_vpss_t(JSONB_OPT_S2J, json, out, sizeof(proto_vsf_vpss_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(in);
        free(out);
    }

    return 0;
}

int msgbox_vsf_venc(msgbox_param_t *param)
{
    proto_vsf_venc_t *in = param->in;
    proto_vsf_venc_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        in = malloc(sizeof(proto_vsf_venc_t));
        out = malloc(sizeof(proto_vsf_venc_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_vsf_venc_t(JSONB_OPT_J2S, json, in, sizeof(proto_vsf_venc_t));
            cJSON_Delete(json);
        }
    }

    if (param->action == PROTO_ACTION_SET) {
        *param->osize = 0;
    } else {
        *param->osize = sizeof(proto_vsf_venc_t);
    }

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_vsf_venc_t(JSONB_OPT_S2J, json, out, sizeof(proto_vsf_venc_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(in);
        free(out);
    }

    return 0;
}