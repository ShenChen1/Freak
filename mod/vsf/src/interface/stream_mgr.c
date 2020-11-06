#include "inc/interface/stream_mgr.h"
#include "cfg/cfg.h"
#include "common.h"
#include "inc/hal/sys.h"
#include "inc/hal/venc.h"
#include "inc/hal/vi.h"
#include "inc/hal/vpss.h"
#include "ufifo.h"

typedef struct {
    ufifo_t *fifo[VSF_STREAM_MAX];
    proto_vsf_stream_t stream[VSF_STREAM_MAX];
} vsf_stream_mgr_priv_t;

static vsf_stream_mgr_t *s_mgr = NULL;

static int __vsf_stream_proc(int id, int type, void *stream, void *args)
{
    return 0;
}

static int __vsf_stream_ctrl(vsf_stream_mgr_t *self, proto_vsf_stream_t *stream)
{
    vsf_stream_mgr_t *mgr       = self;
    vsf_stream_mgr_priv_t *priv = mgr->priv;

    char name[64];
    snprintf(name, sizeof(name), "%d-%d", stream->chn, stream->subchn);
    snprintf(name, sizeof(name), PROTO_VSF_MEDIA_FIFO, name);

    if (stream->enable) {
        if (!priv->fifo[stream->id]) {
            ufifo_init_t init = {
                .lock  = UFIFO_LOCK_NONE,
                .opt   = UFIFO_OPT_ALLOC,
                .alloc = { 1024 * 1024 },
                //.hook  = { recsize, rectag, recput },
            };
            ufifo_open(name, &init, &priv->fifo[stream->id]);
        }
    } else {
        if (priv->fifo[stream->id]) {
            ufifo_close(priv->fifo[stream->id]);
            priv->fifo[stream->id] = NULL;
        }
    }

    memcpy(&priv->stream[stream->id], stream, sizeof(proto_vsf_stream_t));

    vsf_stream_cb_t cb = { NULL, NULL };
    vsf_venc_t *venc = VSF_createVenc(stream->id);
    assert(venc);
    if (stream->enable) {
        cb.args = &priv->stream[stream->id];
        cb.func = __vsf_stream_proc;
    }
    venc->regcallback(venc, &cb);

    return 0;
}

vsf_stream_mgr_t *VSF_createStreamMgr()
{
    vsf_stream_mgr_t *mgr       = s_mgr;
    vsf_stream_mgr_priv_t *priv = NULL;

    if (mgr) {
        return mgr;
    }

    priv = malloc(sizeof(vsf_stream_mgr_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_stream_mgr_priv_t));

    mgr = malloc(sizeof(vsf_stream_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv = priv;
    mgr->ctrl = __vsf_stream_ctrl;

    s_mgr = mgr;
    return mgr;
}