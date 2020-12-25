#include "rtsp/svr_mgr.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    nnm_t req;
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
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(cfg, 0, sizeof(proto_rtsp_svr_cfg_t));
    proto_package_fill(ibuf, cfg->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_GET, PROTO_FORMAT_STRUCTE, cfg, sizeof(proto_rtsp_svr_cfg_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    memcpy(cfg, proto_package_data(obuf), sizeof(proto_rtsp_svr_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __rtsp_svr_set(rtsp_svr_mgr_t *self, proto_rtsp_svr_cfg_t *cfg)
{
    rtsp_svr_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    proto_package_fill(ibuf, cfg->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_SET, PROTO_FORMAT_STRUCTE, cfg, sizeof(proto_rtsp_svr_cfg_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    assert(proto_package_size(obuf) == sizeof(proto_header_t));
    nnm_free(obuf);
    return 0;
}

static int __rtsp_svr_cap(rtsp_svr_mgr_t *self, proto_rtsp_svr_cap_t *cap)
{
    rtsp_svr_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(cap, 0, sizeof(proto_rtsp_svr_cap_t));
    proto_package_fill(ibuf, cap->id, PROTO_RTSP_KEY_SERVER, PROTO_ACTION_CAP, PROTO_FORMAT_STRUCTE, cap, sizeof(proto_rtsp_svr_cap_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_RTSP_KEY_SERVER);
    assert(!obuf->errcode);

    memcpy(cap, proto_package_data(obuf), sizeof(proto_rtsp_svr_cap_t));
    nnm_free(obuf);

    return 0;
}

static int __rtsp_svr_num(rtsp_svr_mgr_t *self)
{
    int ret;
    rtsp_svr_mgr_priv_t *priv = self->priv;
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

rtsp_svr_mgr_t * __weak rtsp_createSvrMgr()
{
    int ret;
    rtsp_svr_mgr_t *mgr       = NULL;
    rtsp_svr_mgr_priv_t *priv = NULL;

    priv = malloc(sizeof(rtsp_svr_mgr_priv_t));
    assert(priv);
    ret = nnm_req_create(PROTO_VSF_COM_NODE, &priv->req);
    assert(!ret);

    mgr = malloc(sizeof(rtsp_svr_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv = priv;
    mgr->destroy = __rtsp_svr_destroy;
    mgr->num = __rtsp_svr_num;
    mgr->cap = __rtsp_svr_cap;
    mgr->get = __rtsp_svr_get;
    mgr->set = __rtsp_svr_set;

    return mgr;
}