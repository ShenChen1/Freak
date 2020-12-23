#ifndef __VSF_STREAM_MGR_H__
#define __VSF_STREAM_MGR_H__

#include "proto_vsf.h"

typedef struct stream_mgr {
    void *priv;
    int (*destroy)(struct stream_mgr *self);

    int (*num)(struct stream_mgr *self);
    int (*cap)(struct stream_mgr *self, proto_vsf_stream_cap_t *cap);
    int (*get)(struct stream_mgr *self, proto_vsf_stream_cfg_t *cfg);
    int (*set)(struct stream_mgr *self, proto_vsf_stream_cfg_t *cfg);

} vsf_stream_mgr_t;

vsf_stream_mgr_t *vsf_createStreamMgr(void);

#endif //__VSF_STREAM_MGR_H__