#include "app/alg_mgr.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    nnm_t req;
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
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(cfg, 0, sizeof(proto_app_alg_cfg_t));
    proto_package_fill(ibuf, 0, PROTO_APP_KEY_ALG, PROTO_ACTION_GET, PROTO_FORMAT_STRUCTE, cfg, sizeof(proto_app_alg_cfg_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    memcpy(cfg, proto_package_data(obuf), sizeof(proto_app_alg_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __app_alg_set(app_alg_mgr_t *self, proto_app_alg_cfg_t *cfg)
{
    app_alg_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    proto_package_fill(ibuf, 0, PROTO_APP_KEY_ALG, PROTO_ACTION_SET, PROTO_FORMAT_STRUCTE, cfg, sizeof(proto_app_alg_cfg_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    assert(proto_package_size(obuf) == sizeof(proto_header_t));
    nnm_free(obuf);
    return 0;
}

static int __app_alg_cap(app_alg_mgr_t *self, proto_app_alg_cap_t *cap)
{
    app_alg_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(cap, 0, sizeof(proto_app_alg_cap_t));
    proto_package_fill(ibuf, 0, PROTO_APP_KEY_ALG, PROTO_ACTION_CAP, PROTO_FORMAT_STRUCTE, cap, sizeof(proto_app_alg_cap_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    memcpy(cap, proto_package_data(obuf), sizeof(proto_app_alg_cap_t));
    nnm_free(obuf);

    return 0;
}

static int __app_alg_num(app_alg_mgr_t *self)
{
    int ret;
    app_alg_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(&ret, 0, sizeof(int));
    proto_package_fill(ibuf, -1, PROTO_APP_KEY_ALG, PROTO_ACTION_NUM, PROTO_FORMAT_STRUCTE, &ret, sizeof(int));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    memcpy(&ret, proto_package_data(obuf), sizeof(int));
    nnm_free(obuf);

    return ret;
}

app_alg_mgr_t * __weak app_createAlgMgr()
{
    int ret;
    app_alg_mgr_t *mgr       = NULL;
    app_alg_mgr_priv_t *priv = NULL;

    priv = malloc(sizeof(app_alg_mgr_priv_t));
    assert(priv);
    ret = nnm_req_create(PROTO_APP_COM_NODE, &priv->req);
    assert(!ret);

    mgr = malloc(sizeof(app_alg_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv = priv;
    mgr->destroy = __app_alg_destroy;
    mgr->num = __app_alg_num;
    mgr->cap = __app_alg_cap;
    mgr->get = __app_alg_get;
    mgr->set = __app_alg_set;

    return mgr;
}