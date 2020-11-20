#include "inc/interface/frame_mgr.h"
#include "cfg/cfg.h"
#include "common.h"
#include "inc/hal/sys.h"
#include "inc/hal/venc.h"
#include "inc/hal/vi.h"
#include "inc/hal/vpss.h"
#include "log.h"
#include "ufifo.h"

typedef struct {
    ufifo_t *fifo[VSF_FRAME_MAX];
    proto_vsf_frame_t info[VSF_FRAME_MAX];
} vsf_frame_mgr_priv_t;

static vsf_frame_mgr_t *s_mgr = NULL;

static unsigned int recsize(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        size = rec->size;
    } else {
        media_record_t rec;
        void *p = (void *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        size = rec.size;
    }

    tracef("size:%u", size);
    return size;
}

static unsigned int rectag(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int tag;
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        tag = rec->tag;
    } else {
        media_record_t rec;
        void *p = (void *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        tag = rec.tag;
    }

    tracef("tag:%x", tag);
    return tag;
}

static unsigned int recput(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg)
{
    size_t totalsize     = sizeof(media_record_t) + sizeof(video_frame_t);
    media_record_t rec   = {};
    video_frame_t *frame = arg;
    unsigned int a = 0, l = 0, _n1 = n1;
    unsigned char *p = NULL, *_p1 = p1, *_p2 = p2;

    rec.size = totalsize;
    rec.tag  = (0xdeadbeef << 8 | 1);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    rec.ts = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

    // copy header
    p = (unsigned char *)(&rec);
    a = sizeof(media_record_t);
    l = min(a, _n1);
    memcpy(_p1, p, l);
    memcpy(_p2, p + l, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    // copy data
    p = (unsigned char *)(frame);
    a = sizeof(video_frame_t);
    l = min(a, _n1);
    memcpy(_p1, p, l);
    memcpy(_p2, p + l, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    tracef("totalsize:%u", totalsize);
    return totalsize;
}

static int __vsf_frame_proc(void *data, void *args)
{
    return 0;
}

static int __vsf_frame_ctrl(vsf_frame_mgr_t *self, proto_vsf_frame_t *info)
{
    vsf_frame_mgr_t *mgr       = self;
    vsf_frame_mgr_priv_t *priv = mgr->priv;

    char name[64];
    snprintf(name, sizeof(name), PROTO_VSF_FRAME_FIFO "%d-%d", info->chn, info->subchn);

    if (info->enable) {
        if (!priv->fifo[info->id]) {
            ufifo_init_t init = {
                .lock  = UFIFO_LOCK_NONE,
                .opt   = UFIFO_OPT_ALLOC,
                .alloc = { priv->info->width * priv->info->height * priv->info->fps + 1024 },
                .hook  = { recsize, rectag, recput },
            };
            ufifo_open(name, &init, &priv->fifo[info->id]);
        }
    } else {
        if (priv->fifo[info->id]) {
            ufifo_close(priv->fifo[info->id]);
            priv->fifo[info->id] = NULL;
        }
    }

    memcpy(&priv->info[info->id], info, sizeof(proto_vsf_frame_t));

    vsf_frame_cb_t cb = { NULL, NULL };
    if (info->enable) {
        cb.args = &priv->info[info->id];
        cb.func = __vsf_frame_proc;
    }

    vsf_vpss_t *vpss = VSF_createVpss(info->chn);
    assert(vpss);
    vpss->regcallback(vpss, info->id, &cb);
    return 0;
}

vsf_frame_mgr_t *VSF_createFrameMgr()
{
    vsf_frame_mgr_t *mgr       = s_mgr;
    vsf_frame_mgr_priv_t *priv = NULL;

    if (mgr) {
        return mgr;
    }

    priv = malloc(sizeof(vsf_frame_mgr_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_frame_mgr_priv_t));

    mgr = malloc(sizeof(vsf_frame_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv = priv;
    mgr->ctrl = __vsf_frame_ctrl;

    s_mgr = mgr;
    return mgr;
}