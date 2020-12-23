#include "vsf/frame_mgr.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    nnm_t req;
} vsf_frame_mgr_priv_t;

static int __vsf_frame_destroy(vsf_frame_mgr_t *self)
{
    vsf_frame_mgr_priv_t *priv = self->priv;

    nnm_req_destory(priv->req);
    free(priv);
    free(self);
    return 0;
}

static int __vsf_frame_get(vsf_frame_mgr_t *self, proto_vsf_frame_cfg_t *cfg)
{
    vsf_frame_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(cfg, 0, sizeof(proto_vsf_frame_cfg_t));
    proto_package_fill(ibuf, cfg->id, PROTO_VSF_KEY_FRAME, PROTO_ACTION_GET, PROTO_FORMAT_STRUCTE, cfg, sizeof(proto_vsf_frame_cfg_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_FRAME);
    assert(!obuf->errcode);

    memcpy(cfg, proto_package_data(obuf), sizeof(proto_vsf_frame_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_frame_set(vsf_frame_mgr_t *self, proto_vsf_frame_cfg_t *cfg)
{
    vsf_frame_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    proto_package_fill(ibuf, cfg->id, PROTO_VSF_KEY_FRAME, PROTO_ACTION_SET, PROTO_FORMAT_STRUCTE, cfg, sizeof(proto_vsf_frame_cfg_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_FRAME);
    assert(!obuf->errcode);

    assert(proto_package_size(obuf) == sizeof(proto_header_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_frame_cap(vsf_frame_mgr_t *self, proto_vsf_frame_cap_t *cap)
{
    vsf_frame_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(cap, 0, sizeof(proto_vsf_frame_cap_t));
    proto_package_fill(ibuf, cap->id, PROTO_VSF_KEY_FRAME, PROTO_ACTION_CAP, PROTO_FORMAT_STRUCTE, cap, sizeof(proto_vsf_frame_cap_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_FRAME);
    assert(!obuf->errcode);

    memcpy(cap, proto_package_data(obuf), sizeof(proto_vsf_frame_cap_t));
    nnm_free(obuf);

    return 0;
}

static int __vsf_frame_num(vsf_frame_mgr_t *self)
{
    int ret;
    vsf_frame_mgr_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(&ret, 0, sizeof(int));
    proto_package_fill(ibuf, -1, PROTO_VSF_KEY_FRAME, PROTO_ACTION_CAP, PROTO_FORMAT_STRUCTE, &ret, sizeof(int));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_FRAME);
    assert(!obuf->errcode);

    memcpy(&ret, proto_package_data(obuf), sizeof(int));
    nnm_free(obuf);

    return ret;
}

vsf_frame_mgr_t * __weak VSF_createFrameMgr()
{
    int ret;
    vsf_frame_mgr_t *mgr       = NULL;
    vsf_frame_mgr_priv_t *priv = NULL;

    priv = malloc(sizeof(vsf_frame_mgr_priv_t));
    assert(priv);
    ret = nnm_req_create(PROTO_VSF_COM_NODE, &priv->req);
    assert(!ret);

    mgr = malloc(sizeof(vsf_frame_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv = priv;
    mgr->destroy = __vsf_frame_destroy;
    mgr->num = __vsf_frame_num;
    mgr->cap = __vsf_frame_cap;
    mgr->get = __vsf_frame_get;
    mgr->set = __vsf_frame_set;

    return mgr;
}