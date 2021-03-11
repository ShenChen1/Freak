#include "common.h"
#include "inc/cfg.h"
#include "log.h"
#include "msgbox.h"
#include "nnm.h"
#include "proto.h"
#include "web/svr_mgr.h"

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize, void *arg)
{
    *out = arg;
    return msgbox_do_handler(in, isize, *out, osize);
}

static int __ws_recv(void *in, size_t isize, void **out, size_t *osize, void *arg)
{
    *out = arg;

    cJSON *json = NULL;
    size_t size = 0;

    char *tmpbuf = calloc(1, PROTO_PACKAGE_MAXSIZE);
    if (tmpbuf == NULL) {
        return -1;
    }

    json = cJSON_Parse((const char *)in);
    jsonb_opt_proto_header_t(JSONB_OPT_J2S, json, tmpbuf, sizeof(proto_header_t));
    cJSON_Delete(json);
    memcpy(proto_package_data(tmpbuf),
           in + isize - (proto_package_size(tmpbuf) - sizeof(proto_header_t)),
           proto_package_size(tmpbuf) - sizeof(proto_header_t));

    msgbox_do_forward(tmpbuf, proto_package_size(tmpbuf), tmpbuf, &size);

    json = cJSON_CreateObject();
    jsonb_opt_proto_header_t(JSONB_OPT_S2J, json, tmpbuf, sizeof(proto_header_t));
    cJSON_PrintPreallocated(json, *out, isize, 0);
    cJSON_Delete(json);

    size = strlen((char *)*out);
    memcpy(*out + size,
           proto_package_data(tmpbuf),
           proto_package_size(tmpbuf) - sizeof(proto_header_t));
    *osize = size + proto_package_size(tmpbuf) - sizeof(proto_header_t);

    free(tmpbuf);
    return 0;
}

int main()
{
    nnm_t rep = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    cfg_load(PROTO_WEB_CFG_PATH);
    extern int msgbox_web_svr(msgbox_param_t *param);
    msgbox_reg_handler(PROTO_WEB_KEY_SVR, msgbox_web_svr);

    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = { __rep_recv, obuf };
    nnm_rep_create(PROTO_WEB_COM_NODE, &init, &rep);

    int i, total;
    web_svr_mgr_t *server = web_createSvrMgr_r();
    total = server->num(server);
    for (i = 0; i < total; i++) {
        proto_web_svr_cfg_t cfg = { .id = i };
        // get cfg
        server->get(server, &cfg);
        // set cfg
        server->set(server, &cfg);
    }
    server->destroy(server);

    nnm_t web_pub = NULL;
    nnm_pub_create(PROTO_WEB_PUB_NODE, &web_pub);
    nnm_t web_rep = NULL;
    static uint8_t rep_obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t rep_init = { __ws_recv, rep_obuf };
    nnm_rep_create(PROTO_WEB_REP_NODE, &rep_init, &web_rep);

    while (1) {
        infof("keep alive");
        sleep(1);
    }

    nnm_rep_destory(rep);
    log_deinit();

    return 0;
}