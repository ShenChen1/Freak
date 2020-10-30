#ifndef __VSF_VENC_H__
#define __VSF_VENC_H__

typedef struct venc {
    void *priv;
    int (*destroy)(struct venc *self);
} vsf_venc_t;

vsf_venc_t *VSF_createVenc(int id);
int VSF_getVencNum(void);

#endif //__VSF_VENC_H__