#include "app/alg_mgr.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    nnm_t req;
    int proto;
} app_alg_mgr_priv_t;

static int __app_alg_destroy(app_alg_mgr_t *self)
{
    app_alg_mgr_priv_t *priv = self->priv;

    nnm_req_destory(priv->req);
    free(priv);
    free(self);
    return 0;
}

static int __app_alg_get(app_alg_mgr_t *self, proto_app_alg_cfg_t *cfg)
{
    app_alg_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_app_alg_cfg_t,
                          cfg,
                          sizeof(proto_app_alg_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_VSF_KEY_FRAME, PROTO_ACTION_GET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_FRAME);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_app_alg_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_app_alg_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __app_alg_set(app_alg_mgr_t *self, proto_app_alg_cfg_t *cfg)
{
    app_alg_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_app_alg_cfg_t,
                          cfg,
                          sizeof(proto_app_alg_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_VSF_KEY_FRAME, PROTO_ACTION_SET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_FRAME);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_app_alg_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_app_alg_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __app_alg_cap(app_alg_mgr_t *self, proto_app_alg_cap_t *cap)
{
    app_alg_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_app_alg_cap_t,
                          cap,
                          sizeof(proto_app_alg_cap_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cap->id, PROTO_VSF_KEY_FRAME, PROTO_ACTION_CAP, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_FRAME);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_app_alg_cfg_t,
                           obuf->data,
                           obuf->size,
                           cap,
                           sizeof(proto_app_alg_cap_t));
    nnm_free(obuf);
    return 0;
}

static int __app_alg_num(app_alg_mgr_t *self)
{
    app_alg_mgr_priv_t *priv = self->priv;
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

    proto_package_fill_header(ibuf, -1, PROTO_VSF_KEY_FRAME, PROTO_ACTION_NUM, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_FRAME);
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

app_alg_mgr_t *__app_createAlgMgr(int proto)
{
    int ret;
    app_alg_mgr_t *mgr       = NULL;
    app_alg_mgr_priv_t *priv = NULL;

    priv = malloc(sizeof(app_alg_mgr_priv_t));
    assert(priv);
    ret = nnm_req_create(PROTO_APP_COM_NODE, &priv->req);
    assert(!ret);
    priv->proto = proto;

    mgr = malloc(sizeof(app_alg_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv    = priv;
    mgr->destroy = __app_alg_destroy;
    mgr->num     = __app_alg_num;
    mgr->cap     = __app_alg_cap;
    mgr->get     = __app_alg_get;
    mgr->set     = __app_alg_set;

    return mgr;
}

app_alg_mgr_t *__weak app_createAlgMgr(void)
{
    return __app_createAlgMgr(PROTO_FORMAT_STRUCTE);
}

app_alg_mgr_t *app_createAlgMgr_r(void)
{
    return __app_createAlgMgr(PROTO_FORMAT_JSON);
}