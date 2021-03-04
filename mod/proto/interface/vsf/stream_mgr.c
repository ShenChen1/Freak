#include "vsf/stream_mgr.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    nnm_t req;
    int proto;
} vsf_stream_mgr_priv_t;

static int __vsf_stream_destroy(vsf_stream_mgr_t *self)
{
    vsf_stream_mgr_priv_t *priv = self->priv;

    nnm_req_destory(priv->req);
    free(priv);
    free(self);
    return 0;
}

static int __vsf_stream_get(vsf_stream_mgr_t *self, proto_vsf_stream_cfg_t *cfg)
{
    vsf_stream_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_vsf_stream_cfg_t,
                          cfg,
                          sizeof(proto_vsf_stream_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_VSF_KEY_STREAM, PROTO_ACTION_GET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_STREAM);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_vsf_stream_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_vsf_stream_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_stream_set(vsf_stream_mgr_t *self, proto_vsf_stream_cfg_t *cfg)
{
    vsf_stream_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_vsf_stream_cfg_t,
                          cfg,
                          sizeof(proto_vsf_stream_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_VSF_KEY_STREAM, PROTO_ACTION_SET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_STREAM);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_vsf_stream_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_vsf_stream_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_stream_cap(vsf_stream_mgr_t *self, proto_vsf_stream_cap_t *cap)
{
    vsf_stream_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_vsf_stream_cap_t,
                          cap,
                          sizeof(proto_vsf_stream_cap_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cap->id, PROTO_VSF_KEY_STREAM, PROTO_ACTION_CAP, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_STREAM);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_vsf_stream_cap_t,
                           obuf->data,
                           obuf->size,
                           cap,
                           sizeof(proto_vsf_stream_cap_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_stream_num(vsf_stream_mgr_t *self)
{
    vsf_stream_mgr_priv_t *priv = self->priv;
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

    proto_package_fill_header(ibuf, -1, PROTO_VSF_KEY_STREAM, PROTO_ACTION_NUM, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_STREAM);
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

vsf_stream_mgr_t *__vsf_createStreamMgr(int proto)
{
    int ret;
    nnm_t req                   = NULL;
    vsf_stream_mgr_t *mgr       = NULL;
    vsf_stream_mgr_priv_t *priv = NULL;

    ret = nnm_req_create(PROTO_VSF_COM_NODE, &req);
    if (ret < 0) {
        return NULL;
    }

    priv = malloc(sizeof(vsf_stream_mgr_priv_t));
    assert(priv);
    priv->proto = proto;
    priv->req   = req;

    mgr = malloc(sizeof(vsf_stream_mgr_t));
    assert(mgr);
    mgr->priv    = priv;
    mgr->destroy = __vsf_stream_destroy;
    mgr->num     = __vsf_stream_num;
    mgr->cap     = __vsf_stream_cap;
    mgr->get     = __vsf_stream_get;
    mgr->set     = __vsf_stream_set;

    return mgr;
}

vsf_stream_mgr_t *__weak vsf_createStreamMgr(void)
{
    return __vsf_createStreamMgr(PROTO_FORMAT_STRUCTE);
}

vsf_stream_mgr_t *vsf_createStreamMgr_r(void)
{
    return __vsf_createStreamMgr(PROTO_FORMAT_JSON);
}