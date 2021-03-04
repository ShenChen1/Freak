#include "web/svr_mgr.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    nnm_t req;
    int proto;
} web_svr_mgr_priv_t;

static int __web_svr_destroy(web_svr_mgr_t *self)
{
    web_svr_mgr_priv_t *priv = self->priv;

    nnm_req_destory(priv->req);
    free(priv);
    free(self);
    return 0;
}

static int __web_svr_get(web_svr_mgr_t *self, proto_web_svr_cfg_t *cfg)
{
    web_svr_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_web_svr_cfg_t,
                          cfg,
                          sizeof(proto_web_svr_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_GET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_web_svr_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_web_svr_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __web_svr_set(web_svr_mgr_t *self, proto_web_svr_cfg_t *cfg)
{
    web_svr_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_web_svr_cfg_t,
                          cfg,
                          sizeof(proto_web_svr_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_SET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_web_svr_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_web_svr_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __web_svr_cap(web_svr_mgr_t *self, proto_web_svr_cap_t *cap)
{
    web_svr_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_web_svr_cap_t,
                          cap,
                          sizeof(proto_web_svr_cap_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cap->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_CAP, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_web_svr_cap_t,
                           obuf->data,
                           obuf->size,
                           cap,
                           sizeof(proto_web_svr_cap_t));
    nnm_free(obuf);
    return 0;
}

static int __web_svr_num(web_svr_mgr_t *self)
{
    web_svr_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf       = NULL;
    size_t osize               = 0;
    size_t isize               = 0;
    proto_num_t num            = {};

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_num_t,
                          &num, sizeof(proto_num_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, -1, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_NUM, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_num_t,
                           obuf->data,
                           obuf->size,
                           &num,
                           sizeof(proto_num_t));
    nnm_free(obuf);
    return num.num;
}

web_svr_mgr_t *__web_createSvrMgr(int proto)
{
    int ret;
    nnm_t req                = NULL;
    web_svr_mgr_t *mgr       = NULL;
    web_svr_mgr_priv_t *priv = NULL;

    ret = nnm_req_create(PROTO_WEB_COM_NODE, &req);
    if (ret < 0) {
        return NULL;
    }

    priv = malloc(sizeof(web_svr_mgr_priv_t));
    assert(priv);
    priv->proto = proto;
    priv->req   = req;

    mgr = malloc(sizeof(web_svr_mgr_t));
    assert(mgr);
    mgr->priv    = priv;
    mgr->destroy = __web_svr_destroy;
    mgr->num     = __web_svr_num;
    mgr->cap     = __web_svr_cap;
    mgr->get     = __web_svr_get;
    mgr->set     = __web_svr_set;

    return mgr;
}

web_svr_mgr_t *__weak web_createSvrMgr(void)
{
    return __web_createSvrMgr(PROTO_FORMAT_STRUCTE);
}

web_svr_mgr_t *web_createSvrMgr_r(void)
{
    return __web_createSvrMgr(PROTO_FORMAT_JSON);
}