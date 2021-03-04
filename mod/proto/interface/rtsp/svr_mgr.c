#include "rtsp/svr_mgr.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    nnm_t req;
    int proto;
} rtsp_svr_mgr_priv_t;

static int __rtsp_svr_destroy(rtsp_svr_mgr_t *self)
{
    rtsp_svr_mgr_priv_t *priv = self->priv;

    nnm_req_destory(priv->req);
    free(priv);
    free(self);
    return 0;
}

static int __rtsp_svr_get(rtsp_svr_mgr_t *self, proto_rtsp_svr_cfg_t *cfg)
{
    rtsp_svr_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_rtsp_svr_cfg_t,
                          cfg,
                          sizeof(proto_rtsp_svr_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_GET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_rtsp_svr_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_rtsp_svr_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __rtsp_svr_set(rtsp_svr_mgr_t *self, proto_rtsp_svr_cfg_t *cfg)
{
    rtsp_svr_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_rtsp_svr_cfg_t,
                          cfg,
                          sizeof(proto_rtsp_svr_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_SET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_rtsp_svr_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_rtsp_svr_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __rtsp_svr_cap(rtsp_svr_mgr_t *self, proto_rtsp_svr_cap_t *cap)
{
    rtsp_svr_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_rtsp_svr_cap_t,
                          cap,
                          sizeof(proto_rtsp_svr_cap_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cap->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_CAP, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_rtsp_svr_cap_t,
                           obuf->data,
                           obuf->size,
                           cap,
                           sizeof(proto_rtsp_svr_cap_t));
    nnm_free(obuf);
    return 0;
}

static int __rtsp_svr_num(rtsp_svr_mgr_t *self)
{
    rtsp_svr_mgr_priv_t *priv = self->priv;
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

rtsp_svr_mgr_t *__rtsp_createSvrMgr(int proto)
{
    int ret;
    nnm_t req                 = NULL;
    rtsp_svr_mgr_t *mgr       = NULL;
    rtsp_svr_mgr_priv_t *priv = NULL;

    ret = nnm_req_create(PROTO_RTSP_COM_NODE, &req);
    if (ret < 0) {
        return NULL;
    }

    priv = malloc(sizeof(rtsp_svr_mgr_priv_t));
    assert(priv);
    priv->proto = proto;
    priv->req   = req;

    mgr = malloc(sizeof(rtsp_svr_mgr_t));
    assert(mgr);
    mgr->priv    = priv;
    mgr->destroy = __rtsp_svr_destroy;
    mgr->num     = __rtsp_svr_num;
    mgr->cap     = __rtsp_svr_cap;
    mgr->get     = __rtsp_svr_get;
    mgr->set     = __rtsp_svr_set;

    return mgr;
}

rtsp_svr_mgr_t *__weak rtsp_createSvrMgr(void)
{
    return __rtsp_createSvrMgr(PROTO_FORMAT_STRUCTE);
}

rtsp_svr_mgr_t *rtsp_createSvrMgr_r(void)
{
    return __rtsp_createSvrMgr(PROTO_FORMAT_JSON);
}