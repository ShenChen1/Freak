
#include "common.h"
#include "log.h"
#include "inc/hal/sys.h"
#include "inc/sdk_cfg.h"

typedef struct {
    int virtid;
    int phyid;
    sdk_sys_info_t *info;
} vsf_sys_priv_t;

typedef struct {
    int num;
    vsf_sys_t **objs;
} vsf_sys_mod_t;

static vsf_sys_mod_t s_mod;

static HI_S32 __sys_vb_init(VB_CONFIG_S *pstVbConfig)
{
    HI_S32 s32Ret = HI_FAILURE;

    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();

    if (NULL == pstVbConfig) {
        errorf("input parameter is null, it is invaild!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VB_SetConfig(pstVbConfig);
    if (HI_SUCCESS != s32Ret) {
        errorf("HI_MPI_VB_SetConfig failed!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VB_Init();
    if (HI_SUCCESS != s32Ret) {
        errorf("HI_MPI_VB_Init failed!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_SYS_Init();
    if (HI_SUCCESS != s32Ret) {
        errorf("HI_MPI_SYS_Init failed!\n");
        HI_MPI_VB_Exit();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static int __sys_set_param(sdk_sys_info_t *info)
{
    int i, ret = 0;
    HI_U64 u64BlkSize;
    VB_CONFIG_S stVbConf = {};
    sdk_sys_vb_info_t *vb_info = &info->stVbConf;

    stVbConf.u32MaxPoolCnt = vb_info->u32MaxPoolCnt;
    for (i = 0; i < vb_info->u32MaxPoolCnt; i++) {
        u64BlkSize = COMMON_GetPicBufferSize(vb_info->astCommPool[i].u32Width,
                                             vb_info->astCommPool[i].u32Height,
                                             PIXEL_FORMAT_YVU_SEMIPLANAR_422,
                                             DATA_BITWIDTH_8,
                                             COMPRESS_MODE_SEG,
                                             DEFAULT_ALIGN);
        stVbConf.astCommPool[i].u64BlkSize = u64BlkSize;
        stVbConf.astCommPool[i].u32BlkCnt  = vb_info->astCommPool[i].u32BlkCnt;
    }

    ret |= __sys_vb_init(&stVbConf);

    return ret;
}

static HI_VOID __sys_exit(void)
{
    HI_MPI_SYS_Exit();
    HI_MPI_VB_ExitModCommPool(VB_UID_VDEC);
    HI_MPI_VB_Exit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __sys_init(vsf_sys_t *self)
{
    vsf_sys_t *obj = self;
    vsf_sys_priv_t *priv = obj->priv;

    return __sys_set_param(priv->info);
}

static int __sys_destroy(vsf_sys_t *self)
{
    vsf_sys_mod_t *mod = &s_mod;
    vsf_sys_t *obj = self;
    vsf_sys_priv_t *priv = obj->priv;

    __sys_exit();
    mod->objs[priv->virtid] = NULL;
    free(priv);
    free(obj);

    return 0;
}

vsf_sys_t *VSF_createSys(int id)
{
    vsf_sys_mod_t *mod = &s_mod;
    vsf_sys_t *obj = NULL;
    vsf_sys_priv_t *priv = NULL;

    if (id >= mod->num) {
        return NULL;
    }

    obj = mod->objs[id];
    if (obj) {
        return obj;
    }

    priv = malloc(sizeof(vsf_sys_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_sys_priv_t));
    priv->virtid = id;
    priv->phyid = sdk_cfg_get_member(as32SysId[0])[id];
    priv->info = sdk_cfg_get_member(astSysInfo[id]);

    obj = malloc(sizeof(vsf_sys_priv_t));
    if (priv == NULL) {
        return NULL;
    }

    obj->priv = priv;
    obj->init = __sys_init;
    obj->destroy = __sys_destroy;

    mod->objs[id] = obj;
    return obj;
}

int VSF_getSysNum(void)
{
    vsf_sys_mod_t *mod = &s_mod;
    return mod->num;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static void __attribute__((constructor(VSF_SYS_PRIORITY))) sdk_sys_constructor()
{
    int i;
    vsf_sys_mod_t *mod = &s_mod;

    mod->num = *sdk_cfg_get_member(s32SysNum);
    mod->objs = calloc(mod->num, sizeof(vsf_sys_t *));
    assert(mod->objs);

    for (i = 0; i < mod->num; i++) {
        vsf_sys_t *sys = VSF_createSys(i);
        assert(!sys->init(sys));
    }
}

static void __attribute__((destructor(VSF_SYS_PRIORITY))) sdk_sys_destructor()
{
    int i;
    vsf_sys_mod_t *mod = &s_mod;

    for (i = 0; i < mod->num; i++) {
        vsf_sys_t *sys = VSF_createSys(i);
        assert(!sys->destroy(sys));
    }

    free(mod->objs);
    mod->num = 0;
}

