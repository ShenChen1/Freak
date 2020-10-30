#ifndef __VSF_SYS_H__
#define __VSF_SYS_H__

typedef struct sys {
    void *priv;
    int (*destroy)(struct sys *self);
} vsf_sys_t;

vsf_sys_t *VSF_createSys(int id);
int VSF_getSysNum(void);

#endif //__VSF_SYS_H__