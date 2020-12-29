#include "vsf/frame_mgr.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    nnm_t req;
    int proto;
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
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_vsf_frame_cfg_t,
                          cfg,
                          sizeof(proto_vsf_frame_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_VSF_KEY_FRAME, PROTO_ACTION_GET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_FRAME);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_vsf_frame_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_vsf_frame_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_frame_set(vsf_frame_mgr_t *self, proto_vsf_frame_cfg_t *cfg)
{
    vsf_frame_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_vsf_frame_cfg_t,
                          cfg,
                          sizeof(proto_vsf_frame_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_VSF_KEY_FRAME, PROTO_ACTION_SET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_FRAME);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_vsf_frame_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_vsf_frame_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_frame_cap(vsf_frame_mgr_t *self, proto_vsf_frame_cap_t *cap)
{
    vsf_frame_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_vsf_frame_cap_t,
                          cap,
                          sizeof(proto_vsf_frame_cap_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cap->id, PROTO_VSF_KEY_FRAME, PROTO_ACTION_CAP, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_FRAME);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_vsf_frame_cfg_t,
                           obuf->data,
                           obuf->size,
                           cap,
                           sizeof(proto_vsf_frame_cap_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_frame_num(vsf_frame_mgr_t *self)
{
    vsf_frame_mgr_priv_t *priv = self->priv;
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

vsf_frame_mgr_t *__vsf_createFrameMgr(int proto)
{
    int ret;
    vsf_frame_mgr_t *mgr       = NULL;
    vsf_frame_mgr_priv_t *priv = NULL;

    priv = malloc(sizeof(vsf_frame_mgr_priv_t));
    assert(priv);
    ret = nnm_req_create(PROTO_VSF_COM_NODE, &priv->req);
    assert(!ret);
    priv->proto = proto;

    mgr = malloc(sizeof(vsf_frame_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv    = priv;
    mgr->destroy = __vsf_frame_destroy;
    mgr->num     = __vsf_frame_num;
    mgr->cap     = __vsf_frame_cap;
    mgr->get     = __vsf_frame_get;
    mgr->set     = __vsf_frame_set;

    return mgr;
}

vsf_frame_mgr_t *__weak vsf_createFrameMgr(void)
{
    return __vsf_createFrameMgr(PROTO_FORMAT_STRUCTE);
}

vsf_frame_mgr_t *vsf_createFrameMgr_r(void)
{
    return __vsf_createFrameMgr(PROTO_FORMAT_JSON);
}