#ifndef __STREAM_MGR_H__
#define __STREAM_MGR_H__

#include "proto.h"

typedef struct stream_mgr {
    void *priv;
    int (*init)(struct stream_mgr *self);
    int (*destroy)(struct stream_mgr *self);

    int (*ctrl)(struct stream_mgr *self, proto_vsf_stream_t *info);

} vsf_stream_mgr_t;

vsf_stream_mgr_t *VSF_createStreamMgr(void);

#endif //__STREAM_MGR_H__