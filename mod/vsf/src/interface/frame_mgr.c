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
    proto_vsf_frame_t info[VSF_FRAME_MAX];
} vsf_frame_mgr_priv_t;

static vsf_frame_mgr_t *s_mgr = NULL;

static int __vsf_frame_getFrame(vsf_frame_mgr_t *self, int id, void *frame, int timeout)
{
    vsf_frame_mgr_t *mgr       = self;
    vsf_frame_mgr_priv_t *priv = mgr->priv;

    vsf_vpss_t *vpss = VSF_createVpss(priv->info[id].chn);
    return vpss->getChnFrame(vpss, id, frame, timeout);
}

static int __vsf_frame_freeFrame(vsf_frame_mgr_t *self, int id, void *frame)
{
    vsf_frame_mgr_t *mgr       = self;
    vsf_frame_mgr_priv_t *priv = mgr->priv;

    vsf_vpss_t *vpss = VSF_createVpss(priv->info[id].chn);
    return vpss->freeChnFrame(vpss, id, frame);
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
    mgr->getFrame = __vsf_frame_getFrame;
    mgr->freeFrame = __vsf_frame_freeFrame;

    s_mgr = mgr;
    return mgr;
}