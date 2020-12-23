#ifndef __VSF_CHN_MGR_H__
#define __VSF_CHN_MGR_H__

#include "proto_vsf.h"

typedef struct chn_mgr {
    void *priv;
    int (*destroy)(struct chn_mgr *self);

    int (*num)(struct chn_mgr *self);
    int (*cap)(struct chn_mgr *self, proto_vsf_chn_cap_t *cap);
    int (*get)(struct chn_mgr *self, proto_vsf_chn_cfg_t *cfg);
    int (*set)(struct chn_mgr *self, proto_vsf_chn_cfg_t *cfg);

} vsf_chn_mgr_t;

vsf_chn_mgr_t *VSF_createChnMgr(void);

#endif //__VSF_CHN_MGR_H__