#include "log.h"
#include "common.h"
#include "sdk.h"
#include "inc/hal/vi.h"
#include "inc/cfg.h"

typedef struct {
    proto_vsf_vi_t cfg;
} vi_priv_t;

static int __vi_set_param()
{
    HI_S32 s32Ret;
    VI_VPSS_MODE_S stVIVPSSMode;

    s32Ret = HI_MPI_SYS_GetVIVPSSMode(&stVIVPSSMode);
    if (HI_SUCCESS != s32Ret) {
        errorf("Get VI-VPSS mode Param failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    stVIVPSSMode.aenMode[0] = VI_OFFLINE_VPSS_OFFLINE;
    s32Ret = HI_MPI_SYS_SetVIVPSSMode(&stVIVPSSMode);
    if (HI_SUCCESS != s32Ret) {
        errorf("Set VI-VPSS mode Param failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static int __vi_set_ctrl_param()
{
    HI_S32 s32Ret;
    ISP_CTRL_PARAM_S stIspCtrlParam;

    s32Ret = HI_MPI_ISP_GetCtrlParam(0, &stIspCtrlParam);
    if (HI_SUCCESS != s32Ret) {
        errorf("HI_MPI_ISP_GetCtrlParam failed with %d!\n", s32Ret);
        return s32Ret;
    }

    stIspCtrlParam.u32StatIntvl = 30 / 30;

    s32Ret = HI_MPI_ISP_SetCtrlParam(0, &stIspCtrlParam);
    if (HI_SUCCESS != s32Ret) {
        errorf("HI_MPI_ISP_SetCtrlParam failed with %d!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static int vi_start(vi_t * self)
{
    __vi_set_param();
    return 0;
}

static int vi_stop(vi_t * self)
{
    return 0;
}

static int vi_set(vi_t *self, void *param, size_t size)
{
    return 0;
}

static int vi_get(vi_t *self, void *param, size_t size)
{
    return 0;
}

vi_t *createVi(int id)
{
    vi_t *obj = NULL;
    vi_priv_t *priv = NULL;

    priv = malloc(sizeof(vi_priv_t));
    memset(priv, 0, sizeof(vi_priv_t));

    obj = malloc(sizeof(vi_t));
    memset(obj, 0, sizeof(vi_t));
    obj->priv = priv;
    obj->start = vi_start;
    obj->stop = vi_stop;
    obj->set = vi_set;
    obj->get = vi_get;

    return obj;
}