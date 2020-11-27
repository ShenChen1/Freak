#include "inc/interface/osd_mgr.h"
#include "cfg/cfg.h"
#include "common.h"
#include "inc/hal/sys.h"
#include "inc/hal/venc.h"
#include "inc/hal/vi.h"
#include "inc/hal/vpss.h"
#include "inc/hal/rgn.h"
#include "log.h"
#include "ufifo.h"

typedef struct {
    proto_vsf_osd_t info[VSF_OSD_MAX];
} vsf_osd_mgr_priv_t;

static vsf_osd_mgr_t *s_mgr = NULL;

static int __vsf_osd_ctrl(vsf_osd_mgr_t *self, proto_vsf_osd_t *info)
{
    vsf_osd_mgr_t *mgr       = self;
    vsf_osd_mgr_priv_t *priv = mgr->priv;

    memcpy(&priv->info[info->id], info, sizeof(proto_vsf_osd_t));
    vsf_rgn_t *rgn = VSF_createRgn(info->id);
    return rgn->ctrl(rgn, &priv->info[info->id]);
}

vsf_osd_mgr_t *VSF_createOsdMgr()
{
    vsf_osd_mgr_t *mgr       = s_mgr;
    vsf_osd_mgr_priv_t *priv = NULL;

    if (mgr) {
        return mgr;
    }

    priv = malloc(sizeof(vsf_osd_mgr_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_osd_mgr_priv_t));

    mgr = malloc(sizeof(vsf_osd_mgr_t));
    if (mgr == NULL) {
        return NULL;
    }

    mgr->priv = priv;
    mgr->ctrl = __vsf_osd_ctrl;

    s_mgr = mgr;
    return mgr;
}