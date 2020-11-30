#include "common.h"
#include "inc/cfg.h"
#include "inc/http.h"
#include "inc/msgbox.h"

int msgbox_http(msgbox_param_t *param)
{
    proto_web_http_t *in  = param->in;
    proto_web_http_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        in  = malloc(sizeof(proto_web_http_t));
        out = malloc(sizeof(proto_web_http_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_web_http_t(JSONB_OPT_J2S, json, in, sizeof(proto_web_http_t));
            cJSON_Delete(json);
        }
    }
    if (param->action == PROTO_ACTION_SET) {
        *param->osize = 0;
        *cfg_get_member(http) = *in;
    } else {
        *param->osize = sizeof(proto_web_http_t);
    }

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_web_http_t(JSONB_OPT_S2J, json, cfg_get_member(http), sizeof(proto_web_http_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(in);
        free(out);
    }

    return 0;
}
