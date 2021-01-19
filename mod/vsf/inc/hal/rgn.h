#ifndef __VSF_RGN_H__
#define __VSF_RGN_H__

typedef struct rgn {
    void *priv;
    int (*init)(struct rgn *self);
    int (*destroy)(struct rgn *self);

    int (*ctrl)(struct rgn *self, int chn, void *cfg);
    int (*trigger)(struct rgn *self, int chn, void *cfg);
} vsf_rgn_t;

vsf_rgn_t *VSF_createRgn(int id);
int VSF_getRgnNum(void);

#endif //__VSF_RGN_H__