#include "app/alg_mgr.h"
#include "common.h"
#include "inc/cfg.h"
#include "inc/hal/face.h"
#include "log.h"
#include "media.h"
#include "proto.h"
#include "vsf/frame_mgr.h"
#include "vsf/osd_mgr.h"

typedef struct {
    void *data;
    size_t size;
} buf_t;

typedef struct {
    mfifo_t *fifos[APP_ITEM_MAX];
    void *osds[APP_ITEM_MAX];
    buf_t bufs[APP_ITEM_MAX];
    proto_app_alg_t *info;
} app_alg_mgr_priv_t;

static app_alg_mgr_t *s_mgr = NULL;

static int __app_alg_get_frame(void *data, void *args)
{
    int ret;
    app_alg_mgr_priv_t *priv = s_mgr->priv;
    proto_app_alg_cfg_t *cfg = args;
    media_record_t *rec      = (media_record_t *)priv->bufs[cfg->id].data;

    ret = priv->fifos[cfg->id]->get(priv->fifos[cfg->id], priv->bufs[cfg->id].data, priv->bufs[cfg->id].size, -1);
    if (ret < 0) {
        return ret;
    }

    void **_data = (void **)data;
    *_data       = (void *)rec->buf;
    return 0;
}

static int __app_alg_free_frame(void *data, void *args)
{
    return 0;
}

static int __app_alg_result(void *data, void *args)
{
    // proto_app_alg_cfg_t *cfg = args;
    // get result and send to osd
    vsf_osd_mgr_t *osd             = args;
    proto_app_alg_result_t *result = data;
    proto_vsf_osd_cfg_t cfg = {
        .id = VSF_OSD_MAX - 1,
        .enable = 1,
        .info = {
            .condition = "objs",
            .objs = {
                .num = result->num,
            },
        },
    };

    int i = 0;
    for (i = 0; i < cfg.info.objs.num; i++) {
        cfg.info.objs.objs[i].id     = result->objs[i].id;
        cfg.info.objs.objs[i].color  = 0x01ff0000;
        cfg.info.objs.objs[i].rect.x = result->objs[i].rect.x;
        cfg.info.objs.objs[i].rect.y = result->objs[i].rect.y;
        cfg.info.objs.objs[i].rect.w = result->objs[i].rect.w;
        cfg.info.objs.objs[i].rect.h = result->objs[i].rect.h;
    }

    if (osd && osd->tgr) {
        osd->tgr(osd, &cfg);
    }

    return 0;
}

static int __app_alg_destroy(app_alg_mgr_t *self)
{
    // never destroy
    return 0;
}

static int __app_alg_set(app_alg_mgr_t *self, proto_app_alg_cfg_t *cfg)
{
    app_alg_mgr_t *mgr       = self;
    app_alg_mgr_priv_t *priv = mgr->priv;
    switch (cfg->type) {
    case ALG_TYPE_FR: {
        if (cfg->enable) {
            vsf_frame_mgr_t *frame_mgr      = vsf_createFrameMgr_r();
            proto_vsf_frame_cap_t frame_cap = { .id = priv->info->caps[cfg->id].frame };
            frame_mgr->cap(frame_mgr, &frame_cap);
            proto_vsf_frame_cfg_t frame_cfg = { .id = priv->info->caps[cfg->id].frame };
            frame_mgr->get(frame_mgr, &frame_cfg);
            frame_cfg.enable = 1;
            frame_cfg.format = VIDEO_FRAME_FORMAT_YUV420P_YVU;
            frame_cfg.width  = 640;
            frame_cfg.height = 640;
            frame_cfg.fps    = 15;
            frame_mgr->set(frame_mgr, &frame_cfg);
            frame_mgr->destroy(frame_mgr);

            mfifo_init_t fifo_init = {
                .type   = MEDIA_VIDEO_FRAME,
                .chn    = frame_cap.chn,
                .subchn = frame_cap.subchn,
            };
            priv->fifos[cfg->id] = mfifo_attach(&fifo_init, 1);
            assert(priv->fifos[cfg->id]);
            priv->fifos[cfg->id]->newest(priv->fifos[cfg->id]);
            priv->osds[cfg->id] = vsf_createOsdMgr_r();

            priv->bufs[cfg->id].size = frame_cfg.width * frame_cfg.height * 3 + 1024;
            priv->bufs[cfg->id].data = malloc(priv->bufs[cfg->id].size);

            app_alg_cb_t cb[APP_ALG_CB_MAX] = {};
            cb[APP_ALG_CB_FRAME_GET].args   = &priv->info->cfgs[cfg->id];
            cb[APP_ALG_CB_FRAME_GET].func   = __app_alg_get_frame;
            cb[APP_ALG_CB_FRAME_FREE].args  = &priv->info->cfgs[cfg->id];
            cb[APP_ALG_CB_FRAME_FREE].func  = __app_alg_free_frame;
            cb[APP_ALG_CB_RESULT_OUT].args  = priv->osds[cfg->id];
            cb[APP_ALG_CB_RESULT_OUT].func  = __app_alg_result;

            app_face_t *hFace = APP_createFaceAlg();
            if (hFace && hFace->regcallback) {
                hFace->regcallback(hFace, cfg->id, cb);
            }
            if (hFace && hFace->init) {
                hFace->init(hFace, cfg->algpath);
            }
        } else {
            if (priv->fifos[cfg->id]) {
                priv->fifos[cfg->id]->destroy(priv->fifos[cfg->id]);
                priv->fifos[cfg->id] = NULL;
            }

            if (priv->bufs[cfg->id].data) {
                free(priv->bufs[cfg->id].data);
                priv->bufs[cfg->id].data = NULL;
            }

            vsf_osd_mgr_t *osd = priv->osds[cfg->id];
            if (osd && osd->destroy) {
                osd->destroy(osd);
            }

            app_face_t *hFace = APP_createFaceAlg();
            if (hFace && hFace->destroy) {
                hFace->destroy(hFace);
            }
        }
        break;
    }
    default: {
        break;
    }
    }
    return 0;
}

static int __app_alg_get(app_alg_mgr_t *self, proto_app_alg_cfg_t *cfg)
{
    app_alg_mgr_t *mgr       = self;
    app_alg_mgr_priv_t *priv = mgr->priv;

    *cfg = priv->info->cfgs[cfg->id];
    return 0;
}

static int __app_alg_cap(app_alg_mgr_t *self, proto_app_alg_cap_t *cap)
{
    app_alg_mgr_t *mgr       = self;
    app_alg_mgr_priv_t *priv = mgr->priv;

    *cap = priv->info->caps[cap->id];
    return 0;
}

static int __app_alg_num(app_alg_mgr_t *self)
{
    app_alg_mgr_t *mgr       = self;
    app_alg_mgr_priv_t *priv = mgr->priv;

    return priv->info->num.num;
}

app_alg_mgr_t *app_createAlgMgr(void)
{
    app_alg_mgr_t *mgr       = s_mgr;
    app_alg_mgr_priv_t *priv = NULL;

    if (mgr) {
        return mgr;
    }

    priv = malloc(sizeof(app_alg_mgr_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(app_alg_mgr_priv_t));
    priv->info = cfg_get_member(alginfo);

    mgr = malloc(sizeof(app_alg_mgr_t));
    assert(mgr);
    memset(mgr, 0, sizeof(app_alg_mgr_t));
    mgr->priv    = priv;
    mgr->destroy = __app_alg_destroy;
    mgr->num     = __app_alg_num;
    mgr->cap     = __app_alg_cap;
    mgr->get     = __app_alg_get;
    mgr->set     = __app_alg_set;

    s_mgr = mgr;
    return mgr;
}
