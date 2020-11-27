#include "inc/cfg.h"
#include "inc/msgbox.h"
#include "inc/interface/osd_mgr.h"

int msgbox_vsf_osd(msgbox_param_t *param)
{
    int ret = 0;
    proto_vsf_osd_t *in  = param->in;
    proto_vsf_osd_t *out = param->out;

    if (param->format == PROTO_FORMAT_JSON) {
        in  = malloc(sizeof(proto_vsf_osd_t));
        out = malloc(sizeof(proto_vsf_osd_t));
        if (param->isize) {
            cJSON *json = cJSON_Parse(param->in);
            jsonb_opt_proto_vsf_osd_t(JSONB_OPT_J2S, json, in, sizeof(proto_vsf_osd_t));
            cJSON_Delete(json);
        }
    }

    if (param->action == PROTO_ACTION_SET) {
        vsf_osd_mgr_t *mgr = VSF_createOsdMgr();
        mgr->ctrl(mgr, in);
        *param->osize = 0;
    } else {
        *param->osize = sizeof(proto_vsf_osd_t);
    }

    if (param->format == PROTO_FORMAT_JSON) {
        if (*param->osize) {
            cJSON *json = cJSON_CreateObject();
            jsonb_opt_proto_vsf_osd_t(JSONB_OPT_S2J, json, out, sizeof(proto_vsf_osd_t));
            cJSON_PrintPreallocated(json, param->out, PROTO_PACKAGE_MAXSIZE, 0);
            cJSON_Delete(json);
            *param->osize = strlen(param->out) + 1;
        }
        free(in);
        free(out);
    }

    return ret;
}
