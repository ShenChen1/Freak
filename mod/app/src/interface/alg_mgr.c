#include "app/alg_mgr.h"
#include "common.h"
#include "inc/cfg.h"
#include "inc/hal/face.h"
#include "log.h"
#include "media.h"
#include "proto.h"
#include "proto_app.h"
#include "ufifo.h"
#include "vsf/frame_mgr.h"
#include "vsf/osd_mgr.h"

typedef struct {
    void *data;
    size_t size;
} buf_t;

typedef struct {
    ufifo_t *fifos[APP_ITEM_MAX];
    void *osds[APP_ITEM_MAX];
    buf_t bufs[APP_ITEM_MAX];
    proto_app_alg_t *info;
} app_alg_mgr_priv_t;

static app_alg_mgr_t *s_mgr = NULL;

static unsigned int recsize(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        size                = rec->size;
    } else {
        media_record_t rec;
        char *p = (char *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        size = rec.size;
    }

    return size;
}

static unsigned int rectag(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int tag;
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        tag                 = rec->tag;
    } else {
        media_record_t rec;
        char *p = (char *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        tag = rec.tag;
    }

    return tag;
}

static unsigned int recget(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg)
{
    media_record_t *rec  = arg;
    unsigned int a = 0, l = 0, _n1 = n1;
    unsigned char *p = NULL, *_p1 = p1, *_p2 = p2;

    // copy header
    p = (unsigned char *)(rec);
    a = sizeof(media_record_t);
    l = min(a, _n1);
    memcpy(p, _p1, l);
    memcpy(p + _n1, _p2, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    // check header
    assert(rec->tag >= (0xdeadbeef << 8));

    // copy data
    p = (unsigned char *)(rec->buf);
    a = rec->size - sizeof(media_record_t);
    l = min(a, _n1);
    memcpy(p, _p1, l);
    memcpy(p + l, _p2, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    return rec->size;
}

static int __app_alg_get_frame(void *data, void *args)
{
    int ret;
    app_alg_mgr_priv_t *priv = s_mgr->priv;
    proto_app_alg_cfg_t *cfg = args;
    media_record_t *rec = (media_record_t *)priv->bufs[cfg->id].data;

    ret = ufifo_get_block(priv->fifos[cfg->id], priv->bufs[cfg->id].data, priv->bufs[cfg->id].size);
    assert(ret > 0 && ret <= priv->bufs[cfg->id].size);

    void **_data = (void **)data;
    *_data = (void *)rec->buf;
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
    int i                          = 0;
    proto_vsf_osd_tgr_t tgr = {
    .id = 4,
    .info = {
        .condition = "objs",
        .objs = {
            .num = result->num,
        },
      },
    };
    for (i = 0; i < tgr.info.objs.num; i++) {
        tgr.info.objs.rects[i].x = result->objs[i].rect.x;
        tgr.info.objs.rects[i].y = result->objs[i].rect.y;
        tgr.info.objs.rects[i].w = result->objs[i].rect.w;
        tgr.info.objs.rects[i].h = result->objs[i].rect.h;
    }

    if (osd && osd->tgr) {
        osd->tgr(osd, &tgr);
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

                char name[64];
                ufifo_init_t init = {
                            .lock = UFIFO_LOCK_NONE,
                            .opt  = UFIFO_OPT_ATTACH,
                            .attach = { .shared = 0, },
                            .hook = { recsize, rectag, NULL, recget },
                };
                snprintf(name, sizeof(name), PROTO_VSF_FRAME_FIFO "%d-%d", frame_cap.chn, frame_cap.subchn);
                ufifo_open(name, &init, &priv->fifos[cfg->id]);
                ufifo_newest(priv->fifos[cfg->id], (0xdeadbeef << 8) | 1);

                vsf_osd_mgr_t *osd  = vsf_createOsdMgr_r();
                priv->osds[cfg->id] = osd;

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
                    ufifo_close(priv->fifos[cfg->id]);
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
