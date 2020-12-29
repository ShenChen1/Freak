#ifndef __VSF_OSD_MGR_H__
#define __VSF_OSD_MGR_H__

#include "proto_vsf.h"

typedef struct osd_mgr {
    void *priv;
    int (*destroy)(struct osd_mgr *self);

    int (*num)(struct osd_mgr *self);
    int (*cap)(struct osd_mgr *self, proto_vsf_osd_cap_t *cap);
    int (*get)(struct osd_mgr *self, proto_vsf_osd_cfg_t *cfg);
    int (*set)(struct osd_mgr *self, proto_vsf_osd_cfg_t *cfg);

} vsf_osd_mgr_t;

vsf_osd_mgr_t *vsf_createOsdMgr(void);
vsf_osd_mgr_t *vsf_createOsdMgr_r(void);

#endif //__VSF_OSD_MGR_H__