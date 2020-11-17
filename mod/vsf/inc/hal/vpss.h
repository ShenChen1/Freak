#ifndef __VSF_VPSS_H__
#define __VSF_VPSS_H__

typedef struct vpss {
    void *priv;
    int (*init)(struct vpss *self);
    int (*destroy)(struct vpss *self);

    int (*getChnFrame)(struct vpss *self, int chn, void *frame, int timeout);
    int (*freeChnFrame)(struct vpss *self, int chn, void *frame);
} vsf_vpss_t;

vsf_vpss_t *VSF_createVpss(int id);
int VSF_getVpssNum(void);

#endif //__VSF_VPSS_H__