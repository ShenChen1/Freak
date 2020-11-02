#include "inc/hal/sys.h"
#include "common.h"
#include "inc/sdk_cfg.h"
#include "log.h"

typedef struct {
    sdk_sys_info_t *info;
} vsf_sys_priv_t;

static vsf_sys_priv_t s_sys_priv;
static vsf_sys_t s_sys_obj;

static HI_S32 __sys_init(VB_CONFIG_S *pstVbConfig)
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
        errorf("HI_MPI_VB_SetConf failed!\n");
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

static HI_VOID __sys_exit(HI_VOID)
{
    HI_MPI_SYS_Exit();
    HI_MPI_VB_ExitModCommPool(VB_UID_VDEC);
    HI_MPI_VB_Exit();
    return;
}

static HI_S32 __sys_set_param(HI_VOID)
{
    int i;
    HI_U64 u64BlkSize;
    VB_CONFIG_S stVbConf;
    sdk_sys_vb_info_t *info = &sdk_cfg_get_member(astSysInfo)->stVbConf;

    for (i = 0; i < info->u32MaxPoolCnt; i++) {
        u64BlkSize = COMMON_GetPicBufferSize(info->astCommPool[i].u32Width,
                                             info->astCommPool[i].u32Height,
                                             PIXEL_FORMAT_YVU_SEMIPLANAR_422,
                                             DATA_BITWIDTH_8,
                                             COMPRESS_MODE_SEG,
                                             DEFAULT_ALIGN);
        stVbConf.astCommPool[i].u64BlkSize = u64BlkSize;
        stVbConf.astCommPool[i].u32BlkCnt  = info->astCommPool[i].u32BlkCnt;
    }

    return __comm_sys_init(&stVbConf);
}

vsf_sys_t *VSF_createSys(int id)
{
    vsf_sys_t *obj       = s_sys_obj;
    vsf_sys_priv_t *priv = NULL;

    return obj;
}

int VSF_getSysNum(void)
{
    return &sdk_cfg_get_member(astSysInfo);
}