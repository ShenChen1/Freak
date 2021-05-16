#include "inc/hal/face.h"
#include "inc/basetype.h"
#include "inc/nnie_face_api.h"
#include "inc/sdk_common.h"
#include "log.h"
#include "common.h"
#include "media.h"
#include "proto_app.h"

extern int tracker_id(int FrameIndex, RESULT_BAG *result_bag, RESULT_BAG *result_out);

typedef struct {
    que_t *resultQue;
    pthread_t s_hMdThread;
    pthread_t s_rsltThread;
    float threshold;
    int isLog;
    HI_BOOL s_bStopSignal;
    app_alg_cb_t cb[APP_ALG_CB_MAX];
    uint8_t *data;
} app_face_priv_t;

typedef struct {
    app_face_t *objs;
} app_face_mod_t;

static app_face_mod_t s_mod = { 0 };

static void *hs_fd_result_task(void *args)
{
    int ret;
    app_face_priv_t *priv = args;
    void *results         = NULL;

    while (HI_FALSE == priv->s_bStopSignal) {
        // block mode
        ret = que_get(priv->resultQue, (ptr_t *)&results, 1);
        if (!ret) {
            priv->cb[APP_ALG_CB_RESULT_OUT].func((void *)results, priv->cb[APP_ALG_CB_RESULT_OUT].args);
            free(results);
        }
    }
    return 0;
}

static void *hs_fd_task(void *args)
{
    IVE_CSC_CTRL_S stCscCtrl;
    IVE_DST_IMAGE_S stDstData;
    IVE_SRC_IMAGE_S stSrcData;
    IVE_HANDLE hIveHandle;

    int i;
    int framecnt = 0;
    int u32Size  = 0;

    HI_S32 s32Ret         = 0;
    HI_BOOL bInstant      = HI_TRUE;
    app_face_priv_t *priv = args;
    HI_U64 alg_phy_addr;
    HI_U64 alg_vir_addr;

    while (HI_FALSE == priv->s_bStopSignal) {
        HI_CHAR *data = NULL;

        if (priv->cb[APP_ALG_CB_FRAME_GET].func) {
            s32Ret = priv->cb[APP_ALG_CB_FRAME_GET].func((void *)&data, priv->cb[APP_ALG_CB_FRAME_GET].args);
        }

        if (HI_SUCCESS != s32Ret) {
            errorf("get failed!\n");
            continue;
        }

        video_frame_t *frame = (video_frame_t *)data;
        HI_CHAR *pVirAddr0   = data + sizeof(video_frame_t);
        HI_CHAR *pVirAddr1   = pVirAddr0 + frame->u32Stride[0] * frame->u32Height;

        alg_vir_addr = (HI_U64)(size_t)pVirAddr0;
        alg_phy_addr = frame->u64PhyAddr[0];
        if (0 == framecnt) {
            s32Ret = HI_MPI_SYS_MmzAlloc_Cached(&stDstData.au64PhyAddr[0],
                                                (HI_VOID **)&stDstData.au64VirAddr[0],
                                                "User",
                                                HI_NULL,
                                                frame->u32Stride[0] * frame->u32Height * 3); // 640*640×3
            if (HI_SUCCESS != s32Ret) {
                HI_MPI_SYS_MmzFree(stSrcData.au64PhyAddr[0], (HI_VOID *)(size_t)stSrcData.au64VirAddr[0]);
                errorf("HI_MPI_SYS_MmzAlloc_Cached Failed!");
                return NULL;
            }
        }

        if (VIDEO_FRAME_FORMAT_YUV420SP_VU == frame->enPixelFormat) {
            stCscCtrl.enMode         = IVE_CSC_MODE_VIDEO_BT601_YUV2RGB;
            stSrcData.enType         = IVE_IMAGE_TYPE_YUV420SP;
            stSrcData.au64PhyAddr[0] = frame->u64PhyAddr[0];      // 0
            stSrcData.au64PhyAddr[1] = frame->u64PhyAddr[1];      //
            stSrcData.au64VirAddr[0] = (HI_U64)(size_t)pVirAddr0; // 0
            stSrcData.au64VirAddr[1] = (HI_U64)(size_t)pVirAddr1; //
            stSrcData.au32Stride[0]  = frame->u32Stride[0];       // 640
            stSrcData.au32Stride[1]  = frame->u32Stride[1];       // 640
            stSrcData.u32Width       = frame->u32Width;           // 640
            stSrcData.u32Height      = frame->u32Height;          // 640

            stDstData.enType        = IVE_IMAGE_TYPE_U8C3_PLANAR;
            stDstData.u32Width      = stSrcData.u32Width;
            stDstData.u32Height     = stSrcData.u32Height;
            stDstData.au32Stride[0] = stSrcData.au32Stride[0]; // 640
            stDstData.au32Stride[1] = stSrcData.au32Stride[0];
            stDstData.au32Stride[2] = stSrcData.au32Stride[0];
            u32Size                 = stDstData.au32Stride[0] * stDstData.u32Height;

            memset((HI_VOID *)(size_t)stDstData.au64VirAddr[0], 0, stDstData.u32Height * stDstData.au32Stride[0] * 3);
            s32Ret = HI_MPI_SYS_MmzFlushCache(stDstData.au64PhyAddr[0],
                                              (HI_VOID *)(size_t)stDstData.au64VirAddr[0],
                                              stDstData.u32Height * stDstData.au32Stride[0] * 3);
            if (HI_SUCCESS != s32Ret) {
                HI_MPI_SYS_MmzFree(stDstData.au64PhyAddr[0], (HI_VOID *)(size_t)stDstData.au64VirAddr[0]);
                return NULL;
            }

            stDstData.au64PhyAddr[1] = stDstData.au64PhyAddr[0] + u32Size;
            stDstData.au64PhyAddr[2] = stDstData.au64PhyAddr[0] + u32Size * 2;
            stDstData.au64VirAddr[1] = stDstData.au64VirAddr[0] + u32Size;
            stDstData.au64VirAddr[2] = stDstData.au64VirAddr[0] + u32Size * 2;
            s32Ret = HI_MPI_IVE_CSC(&hIveHandle, &stSrcData, &stDstData, &stCscCtrl, bInstant);
            if (HI_SUCCESS != s32Ret) {
                errorf("s32Ret:%d \n", s32Ret);
                return NULL;
            }
            alg_vir_addr = stDstData.au64VirAddr[0];
            alg_phy_addr = stDstData.au64PhyAddr[0];
        }

        NNIE_IMG_RETINAFACE frame_alg;
        frame_alg.au64PhyAddr[0] = (size_t)alg_phy_addr;
        frame_alg.au64VirAddr[0] = (size_t)alg_vir_addr;

        NNIE_FD_RESULT result;
        memset(&result, 0, sizeof(NNIE_FD_RESULT));
        NNIE_FACE_DETECTOR_GET_NEW(&frame_alg, &result);

        RESULT_BAG result_bag;
        RESULT_BAG result_out;
        memset(&result_bag, 0, sizeof(RESULT_BAG));
        memset(&result_out, 0, sizeof(RESULT_BAG));

        for (i = 0; i < result.fd_num; i++) {
            result.fd[i].ul.x = range(result.fd[i].ul.x, 0, 640);
            result.fd[i].ul.y = range(result.fd[i].ul.y, 0, 640);

            result_bag.obj[i].centerx = (result.fd[i].ul.x + result.fd[i].lr.x) / 2;
            result_bag.obj[i].centery = (result.fd[i].ul.y + result.fd[i].lr.y) / 2;
            result_bag.obj[i].heigth  = result.fd[i].lr.y - result.fd[i].ul.y;
            result_bag.obj[i].width   = result.fd[i].lr.x - result.fd[i].ul.x;
        }

        result_bag.obj_num = result.fd_num;
        if (result_bag.obj_num > 0) {
            framecnt++;
        }

        tracker_id(framecnt, &result_bag, &result_out);

        if (priv->cb[APP_ALG_CB_RESULT_OUT].func) {
            proto_app_alg_result_t *results = malloc(sizeof(proto_app_alg_result_t));
            if (results) {
                memset(results, 0, sizeof(proto_app_alg_result_t));
                results->num = result_bag.obj_num;
                for (i = 0; i < result_out.obj_num; i++) {
                    results->objs[i].id     = result_out.obj[i].ID;
                    results->objs[i].rect.x = result_out.obj[i].centerx - result_out.obj[i].width / 2;
                    results->objs[i].rect.y = result_out.obj[i].centery - result_out.obj[i].heigth / 2;
                    results->objs[i].rect.w = result_out.obj[i].width;
                    results->objs[i].rect.h = result_out.obj[i].heigth;
                    // to do keypoint
                    results->objs[i].rect.x = results->objs[i].rect.x * 8192 / 640;
                    results->objs[i].rect.y = results->objs[i].rect.y * 8192 / 640;
                    results->objs[i].rect.w = results->objs[i].rect.w * 8192 / 640;
                    results->objs[i].rect.h = results->objs[i].rect.h * 8192 / 640;
                }
                // no block
                que_put(priv->resultQue, results, 0);
            }
        }

        if (priv->cb[APP_ALG_CB_FRAME_GET].func) {
            s32Ret = priv->cb[APP_ALG_CB_FRAME_GET].func(data, priv->cb[APP_ALG_CB_FRAME_GET].args);
        }
    }

    HI_MPI_SYS_MmzFree(stDstData.au64PhyAddr[0], (HI_VOID *)(size_t)stDstData.au64VirAddr[0]);
    return 0;
}

static int __hs_fd_init(app_face_t *self, char *path)
{
    app_face_t *obj       = self;
    app_face_priv_t *priv = obj->priv;
    infof("fd_init");

    priv->isLog         = 0;
    priv->threshold     = 0.6;
    priv->s_bStopSignal = HI_FALSE;

    char *pcModelName = "/userdata/data/nnie_model/face/mnet_640_inst.wk";
    NNIE_FACE_DETECTOR_INIT(pcModelName, priv->threshold, priv->isLog);
    que_create(32, &priv->resultQue);

    pthread_create(&priv->s_hMdThread, NULL, hs_fd_task, (void *)priv);
    pthread_create(&priv->s_rsltThread, NULL, hs_fd_result_task, (void *)priv);
    return 0;
}

static int __hs_fd_regcallback(app_face_t *self, int type, app_alg_cb_t *cb)
{
    app_face_t *obj       = self;
    app_face_priv_t *priv = obj->priv;

    if (cb == NULL) {
        priv->cb[APP_ALG_CB_FRAME_GET].args  = NULL;
        priv->cb[APP_ALG_CB_FRAME_GET].func  = NULL;
        priv->cb[APP_ALG_CB_FRAME_FREE].args = NULL;
        priv->cb[APP_ALG_CB_FRAME_FREE].func = NULL;
        priv->cb[APP_ALG_CB_RESULT_OUT].args = NULL;
        priv->cb[APP_ALG_CB_RESULT_OUT].func = NULL;
    } else {
        priv->cb[APP_ALG_CB_FRAME_GET].args  = cb[APP_ALG_CB_FRAME_GET].args;
        priv->cb[APP_ALG_CB_FRAME_GET].func  = cb[APP_ALG_CB_FRAME_GET].func;
        priv->cb[APP_ALG_CB_FRAME_FREE].args = cb[APP_ALG_CB_FRAME_FREE].args;
        priv->cb[APP_ALG_CB_FRAME_FREE].func = cb[APP_ALG_CB_FRAME_FREE].func;
        priv->cb[APP_ALG_CB_RESULT_OUT].args = cb[APP_ALG_CB_RESULT_OUT].args;
        priv->cb[APP_ALG_CB_RESULT_OUT].func = cb[APP_ALG_CB_RESULT_OUT].func;
    }

    return 0;
}

static int __hs_fd_destroy(app_face_t *self)
{
    app_face_t *obj       = self;
    app_face_priv_t *priv = obj->priv;

    priv->s_bStopSignal = HI_TRUE;
    pthread_join(priv->s_hMdThread, NULL);
    pthread_join(priv->s_rsltThread, NULL);
    NNIE_FACE_DETECTOR_RELEASE();
    que_delete(priv->resultQue);
    return 0;
}

app_face_t *APP_createFaceAlg(void)
{
    app_face_mod_t *mod   = &s_mod;
    app_face_t *obj       = NULL;
    app_face_priv_t *priv = NULL;

    obj = mod->objs;
    if (obj) {
        return obj;
    }

    priv = malloc(sizeof(app_face_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(app_face_priv_t));
    priv->s_bStopSignal = HI_TRUE;

    obj = malloc(sizeof(app_face_t));
    assert(obj);
    memset(obj, 0, sizeof(app_face_t));

    obj->priv        = priv;
    obj->init        = __hs_fd_init;
    obj->destroy     = __hs_fd_destroy;
    obj->regcallback = __hs_fd_regcallback;

    mod->objs = obj;
    return obj;
}
