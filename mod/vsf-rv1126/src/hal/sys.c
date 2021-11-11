/*
 * @Author: your name
 * @Date: 2021-11-05 11:20:27
 * @LastEditTime: 2021-11-11 09:52:00
 * @LastEditors: your name
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \Freak\mod\vsf-rv1126\src\hal\sys.c
 */

#include "common.h"
#include "log.h"
#include "inc/hal/sys.h"
#include "inc/sdk_cfg.h"

typedef struct {
    int res;//using easymedia ,there is nothing
} vsf_sys_priv_t;

typedef struct {
    vsf_sys_t *obj;
} vsf_sys_mod_t;

static vsf_sys_mod_t s_mod;

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __sys_init(vsf_sys_t *self)
{
    //using easymedia

    return RK_MPI_SYS_Init();
}

static int __sys_destroy(vsf_sys_t *self)
{
    vsf_sys_t *obj = self;
    vsf_sys_priv_t *priv = obj->priv;

    free(priv);
    free(obj);

    return 0;
}

vsf_sys_t *VSF_createSys(int id)
{
    vsf_sys_mod_t *mod = &s_mod;
    vsf_sys_t *obj = NULL;
    vsf_sys_priv_t *priv = NULL;

    obj = mod->obj;
    if (obj) {
        return obj;
    }

    priv = malloc(sizeof(vsf_sys_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_sys_priv_t));

    obj = malloc(sizeof(vsf_sys_t));
    assert(obj);
    memset(obj, 0, sizeof(vsf_sys_t));
    obj->priv = priv;
    obj->init = __sys_init;
    obj->destroy = __sys_destroy;

    mod->obj = obj;
    return obj;
}



////////////////////////////////////////////////////////////////////////////////////////////////////

static void __attribute__((constructor(VSF_SYS_PRIORITY))) sdk_sys_constructor()
{
    vsf_sys_t *sys = VSF_createSys(0);
    assert(!sys->init(sys));
    
}

static void __attribute__((destructor(VSF_SYS_PRIORITY))) sdk_sys_destructor()
{

    vsf_sys_t *sys = VSF_createSys(0);
    assert(!sys->destroy(sys));
}

