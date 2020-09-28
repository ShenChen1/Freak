#include "common.h"
#include "inc/dummy.h"
#include "inc/msgbox.h"
#include "inc/cfg.h"

int msgbox_dummy(msgbox_param_t *param)
{
    int ret;
    dummy_t *obj = NULL;
    proto_bsp_dummy_t *in = param->in;
    proto_bsp_dummy_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        in = malloc(sizeof(proto_bsp_dummy_t));
        out = malloc(sizeof(proto_bsp_dummy_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_bsp_dummy_t(JSONB_OPT_J2S, json, in, sizeof(proto_bsp_dummy_t));
            cJSON_Delete(json);
        }
    }

    obj = createDummy(param->chn);
    if (obj == NULL) {
        return -EINVAL;
    }

    if (param->action == PROTO_ACTION_SET) {
        ret = obj->set(obj, in->value);
        *param->osize = 0;
        *cfg_get_member(dummy[param->chn]) = *in;
    } else {
        ret = obj->get(obj, &out->value);
        *param->osize = sizeof(proto_bsp_dummy_t);
    }

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_bsp_dummy_t(JSONB_OPT_S2J, json, cfg_get_member(dummy[param->chn]), sizeof(proto_bsp_dummy_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(in);
        free(out);
    }

    return ret;
}
