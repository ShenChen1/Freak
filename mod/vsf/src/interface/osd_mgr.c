#include "vsf/osd_mgr.h"
#include "inc/cfg.h"
#include "inc/hal/rgn.h"
#include "inc/hal/sys.h"
#include "inc/hal/venc.h"
#include "inc/hal/vi.h"
#include "inc/hal/vpss.h"
#include "log.h"
#include "proto.h"
#include "ufifo.h"

typedef struct {
    proto_vsf_osd_t *info;
} vsf_osd_mgr_priv_t;

static vsf_osd_mgr_t *s_mgr = NULL;

static int __vsf_osd_destroy(vsf_osd_mgr_t *self)
{
    // never destroy
    return 0;
}

static int __vsf_osd_set(vsf_osd_mgr_t *self, proto_vsf_osd_cfg_t *cfg)
{
    // vsf_osd_mgr_t *mgr       = self;
    // vsf_osd_mgr_priv_t *priv = mgr->priv;

    vsf_rgn_t *rgn = VSF_createRgn(cfg->id);
    return rgn->ctrl(rgn, cfg);
}

static int __vsf_osd_get(vsf_osd_mgr_t *self, proto_vsf_osd_cfg_t *cfg)
{
    vsf_osd_mgr_t *mgr       = self;
    vsf_osd_mgr_priv_t *priv = mgr->priv;

    *cfg = priv->info->cfgs[cfg->id];
    return 0;
}

static int __vsf_osd_cap(vsf_osd_mgr_t *self, proto_vsf_osd_cap_t *cap)
{
    vsf_osd_mgr_t *mgr       = self;
    vsf_osd_mgr_priv_t *priv = mgr->priv;

    *cap = priv->info->caps[cap->id];
    return 0;
}

static int __vsf_osd_num(vsf_osd_mgr_t *self)
{
    vsf_osd_mgr_t *mgr       = self;
    vsf_osd_mgr_priv_t *priv = mgr->priv;

    return priv->info->num;
}

vsf_osd_mgr_t *vsf_createOsdMgr()
{
    vsf_osd_mgr_t *mgr       = s_mgr;
    vsf_osd_mgr_priv_t *priv = NULL;

    if (mgr) {
        return mgr;
    }

    priv = malloc(sizeof(vsf_osd_mgr_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_osd_mgr_priv_t));
    priv->info = cfg_get_member(osd);

    mgr = malloc(sizeof(vsf_osd_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv    = priv;
    mgr->destroy = __vsf_osd_destroy;
    mgr->set     = __vsf_osd_set;
    mgr->get     = __vsf_osd_get;
    mgr->cap     = __vsf_osd_cap;
    mgr->num     = __vsf_osd_num;

    s_mgr = mgr;
    return mgr;
}