#include "vsf/osd_mgr.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    nnm_t req;
    int proto;
} vsf_osd_mgr_priv_t;

static int __vsf_osd_destroy(vsf_osd_mgr_t *self)
{
    vsf_osd_mgr_priv_t *priv = self->priv;

    nnm_req_destory(priv->req);
    free(priv);
    free(self);
    return 0;
}

static int __vsf_osd_get(vsf_osd_mgr_t *self, proto_vsf_osd_cfg_t *cfg)
{
    vsf_osd_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf     = NULL;
    size_t osize             = 0;
    size_t isize             = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_vsf_osd_cfg_t,
                          cfg,
                          sizeof(proto_vsf_osd_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_VSF_KEY_OSD, PROTO_ACTION_GET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_OSD);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_vsf_osd_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_vsf_osd_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_osd_set(vsf_osd_mgr_t *self, proto_vsf_osd_cfg_t *cfg)
{
    vsf_osd_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf     = NULL;
    size_t osize             = 0;
    size_t isize             = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_vsf_osd_cfg_t,
                          cfg,
                          sizeof(proto_vsf_osd_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cfg->id, PROTO_VSF_KEY_OSD, PROTO_ACTION_SET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_OSD);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_vsf_osd_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_vsf_osd_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_osd_cap(vsf_osd_mgr_t *self, proto_vsf_osd_cap_t *cap)
{
    vsf_osd_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf     = NULL;
    size_t osize             = 0;
    size_t isize             = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_vsf_osd_cap_t,
                          cap,
                          sizeof(proto_vsf_osd_cap_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, cap->id, PROTO_VSF_KEY_OSD, PROTO_ACTION_CAP, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_OSD);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_vsf_osd_cfg_t,
                           obuf->data,
                           obuf->size,
                           cap,
                           sizeof(proto_vsf_osd_cap_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_osd_tgr(vsf_osd_mgr_t *self, proto_vsf_osd_tgr_t *tgr)
{
    vsf_osd_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf     = NULL;
    size_t osize             = 0;
    size_t isize             = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_vsf_osd_tgr_t,
                          tgr,
                          sizeof(proto_vsf_osd_tgr_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, tgr->id, PROTO_VSF_KEY_OSD, PROTO_ACTION_TGR, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_OSD);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_vsf_osd_tgr_t,
                           obuf->data,
                           obuf->size,
                           tgr,
                           sizeof(proto_vsf_osd_tgr_t));
    nnm_free(obuf);
    return 0;
}

static int __vsf_osd_num(vsf_osd_mgr_t *self)
{
    vsf_osd_mgr_priv_t *priv = self->priv;
    proto_header_t *obuf     = NULL;
    size_t osize             = 0;
    size_t isize             = 0;
    proto_num_t num          = {};

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_num_t,
                          &num, sizeof(proto_num_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, -1, PROTO_VSF_KEY_OSD, PROTO_ACTION_NUM, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_VSF_KEY_OSD);
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

vsf_osd_mgr_t *__vsf_createOsdMgr(int proto)
{
    int ret;
    vsf_osd_mgr_t *mgr       = NULL;
    vsf_osd_mgr_priv_t *priv = NULL;

    priv = malloc(sizeof(vsf_osd_mgr_priv_t));
    assert(priv);
    ret = nnm_req_create(PROTO_VSF_COM_NODE, &priv->req);
    assert(!ret);
    priv->proto = proto;

    mgr = malloc(sizeof(vsf_osd_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv    = priv;
    mgr->destroy = __vsf_osd_destroy;
    mgr->num     = __vsf_osd_num;
    mgr->cap     = __vsf_osd_cap;
    mgr->get     = __vsf_osd_get;
    mgr->set     = __vsf_osd_set;
    mgr->tgr     = __vsf_osd_tgr;

    return mgr;
}

vsf_osd_mgr_t *__weak vsf_createOsdMgr(void)
{
    return __vsf_createOsdMgr(PROTO_FORMAT_STRUCTE);
}

vsf_osd_mgr_t *vsf_createOsdMgr_r(void)
{
    return __vsf_createOsdMgr(PROTO_FORMAT_JSON);
}