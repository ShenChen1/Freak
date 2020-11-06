#include "inc/cfg.h"
#include "inc/msgbox.h"

int msgbox_vsf_stream(msgbox_param_t *param)
{
    int ret = 0;
    proto_vsf_stream_t *in  = param->in;
    proto_vsf_stream_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        in  = malloc(sizeof(proto_vsf_stream_t));
        out = malloc(sizeof(proto_vsf_stream_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_vsf_stream_t(JSONB_OPT_J2S, json, in, sizeof(proto_vsf_stream_t));
            cJSON_Delete(json);
        }
    }

    if (param->action == PROTO_ACTION_SET) {
        *param->osize = 0;
    } else {
        *param->osize = sizeof(proto_vsf_stream_t);
    }

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_vsf_stream_t(JSONB_OPT_S2J, json, out, sizeof(proto_vsf_stream_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(in);
        free(out);
    }

    return ret;
}
