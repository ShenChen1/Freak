#ifndef __VSF_VPSS_H__
#define __VSF_VPSS_H__

typedef struct vpss {
    void *priv;
    int (*destroy)(struct vpss *self);
} vsf_vpss_t;

vsf_vpss_t *createVpss(int id);
int VSF_getViNum(void);

#endif //__VSF_VPSS_H__