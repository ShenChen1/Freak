#include "web/svr_mgr.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    nnm_t req;
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
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(cfg, 0, sizeof(proto_web_svr_cfg_t));
    proto_package_fill(ibuf, cfg->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_GET, PROTO_FORMAT_STRUCTE, cfg, sizeof(proto_web_svr_cfg_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    memcpy(cfg, proto_package_data(obuf), sizeof(proto_web_svr_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __web_svr_set(web_svr_mgr_t *self, proto_web_svr_cfg_t *cfg)
{
    web_svr_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    proto_package_fill(ibuf, cfg->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_SET, PROTO_FORMAT_STRUCTE, cfg, sizeof(proto_web_svr_cfg_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    assert(proto_package_size(obuf) == sizeof(proto_header_t));
    nnm_free(obuf);
    return 0;
}

static int __web_svr_cap(web_svr_mgr_t *self, proto_web_svr_cap_t *cap)
{
    web_svr_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(cap, 0, sizeof(proto_web_svr_cap_t));
    proto_package_fill(ibuf, cap->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_CAP, PROTO_FORMAT_STRUCTE, cap, sizeof(proto_web_svr_cap_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    memcpy(cap, proto_package_data(obuf), sizeof(proto_web_svr_cap_t));
    nnm_free(obuf);

    return 0;
}

static int __web_svr_num(web_svr_mgr_t *self)
{
    int ret;
    web_svr_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(&ret, 0, sizeof(int));
    proto_package_fill(ibuf, -1, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_CAP, PROTO_FORMAT_STRUCTE, &ret, sizeof(int));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    memcpy(&ret, proto_package_data(obuf), sizeof(int));
    nnm_free(obuf);

    return ret;
}

web_svr_mgr_t * __weak web_createSvrMgr()
{
    int ret;
    web_svr_mgr_t *mgr       = NULL;
    web_svr_mgr_priv_t *priv = NULL;

    priv = malloc(sizeof(web_svr_mgr_priv_t));
    assert(priv);
    ret = nnm_req_create(PROTO_VSF_COM_NODE, &priv->req);
    assert(!ret);

    mgr = malloc(sizeof(web_svr_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv = priv;
    mgr->destroy = __web_svr_destroy;
    mgr->num = __web_svr_num;
    mgr->cap = __web_svr_cap;
    mgr->get = __web_svr_get;
    mgr->set = __web_svr_set;

    return mgr;
}