#ifndef __VSF_VI_H__
#define __VSF_VI_H__

typedef struct vi {
    void *priv;
    int (*init)(struct vi *self);
    int (*destroy)(struct vi *self);
} vsf_vi_t;

vsf_vi_t *VSF_createVi(int id);
int VSF_getViNum(void);

#endif //__VSF_VI_H__