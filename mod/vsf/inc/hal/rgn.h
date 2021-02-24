#ifndef __VSF_RGN_H__
#define __VSF_RGN_H__

#include <stdint.h>

typedef enum {
    VSF_RGN_COVER,
    VSF_RGN_BITMAP,
} vsf_rgn_type_e;

typedef struct {
    int32_t x;
    int32_t y;
} vsf_rgn_point_t;

typedef struct {
    uint32_t color;
    vsf_rgn_point_t points[4];
} vsf_rgn_cover_t;

typedef struct {
    uint32_t enPixelFmt;
    uint32_t u32Width;
    uint32_t u32Height;
    uint32_t u32Stride;
    void *pData;
} vsf_rgn_bitmap_t;

typedef int (*vsf_rgn_proc_t)(void *rgn, void *args);

typedef struct {
    void *args;
    vsf_rgn_proc_t proc;
} vsf_rgn_cb_t;

typedef struct {
    int enable;
    int layer;
    int type;
    int chn;
    union {
        vsf_rgn_cb_t bitmap;
        vsf_rgn_cover_t cover;
    };
} vsf_rgn_cfg_t;

typedef struct rgn {
    void *priv;
    int (*init)(struct rgn *self);
    int (*destroy)(struct rgn *self);

    int (*ctrl)(struct rgn *self, vsf_rgn_cfg_t *cfg);
} vsf_rgn_t;

vsf_rgn_t *VSF_createRgn(int id);
int VSF_getRgnNum(void);

#endif //__VSF_RGN_H__