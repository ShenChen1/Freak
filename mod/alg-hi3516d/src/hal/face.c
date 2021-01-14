#include "inc/hal/face.h"
#include "common.h"
#include "hi_comm_ive.h"
#include "inc/basetype.h"
#include "inc/nnie_face_api.h"
#include "log.h"
#include "media.h"
#include "mpi_ive.h"
#include "sample/sample_comm.h"
#include "ufifo.h"

typedef struct {
    ufifo_t *fifo_get;
    ufifo_t *fifo_free;
    pthread_t s_hMdThread;
    float threshold;
    int isLog;
    HI_BOOL s_bStopSignal;
    app_alg_cb_t cb[APP_ALG_CB_MAX];
    uint8_t data[1024]; // img manager
} app_face_priv_t;

typedef struct {
    app_face_t *objs;
} app_face_mod_t;

static app_face_mod_t s_mod = { 0 };

extern int tracker_id(int FrameIndex, RESULT_BAG *result_bag, RESULT_BAG *result_out);

int saveBMPFile(unsigned char *src, int width, int height, const char *name)
{
    BMPHeader hdr;
    BMPInfoHeader infoHdr;

    FILE *fp = NULL;
    if (NULL == src) {
        return (-1);
    }

    fp = fopen(name, "wb");
    if (NULL == fp) {
        printf("saveBMPFile: Err: Open!\n");
        return (-1);
    }

    infoHdr.size          = sizeof(BMPInfoHeader);
    infoHdr.width         = width;
    infoHdr.height        = 0 - height;
    infoHdr.planes        = 1;
    infoHdr.bitsPerPixel  = 24;
    infoHdr.compression   = 0;
    infoHdr.imageSize     = width * height;
    infoHdr.xPelsPerMeter = 0;
    infoHdr.yPelsPerMeter = 0;
    infoHdr.clrUsed       = 0;
    infoHdr.clrImportant  = 0;

    hdr.type      = 0x4D42;
    hdr.size      = 54 + infoHdr.imageSize * 3;
    hdr.reserved1 = 0;
    hdr.reserved2 = 0;
    hdr.offset    = 54;

    // BGR2RGB
    int i;
    unsigned char do_save[infoHdr.imageSize * 3];
    for (i = 0; i < width * height; i++) {
        // R
        do_save[i * 3] = src[i];
        // G
        do_save[i * 3 + 1] = src[infoHdr.imageSize + i];
        // B
        do_save[i * 3 + 2] = src[infoHdr.imageSize * 2 + i];
    }

    fwrite(&hdr, sizeof(hdr), 1, fp);
    fwrite(&infoHdr, sizeof(infoHdr), 1, fp);
    fwrite(do_save, sizeof(unsigned char), width * height * 3, fp); // TODO:
    fflush(fp);

    if (ferror(fp)) {
        printf("saveBMPFile: Err: Unknown!***\n");
    }

    fclose(fp);
    fp = NULL;

    return 0;
}

static void *hs_fd_task(void *args)
{
    IVE_CSC_CTRL_S stCscCtrl;
    IVE_DST_IMAGE_S stDstData;
    IVE_SRC_IMAGE_S stSrcData;
    IVE_HANDLE hIveHandle;

    int i, framecnt, Size0, Size1, u32Size = 0;
    HI_S32 s32Ret         = 0;
    HI_BOOL bInstant      = HI_TRUE;
    app_face_priv_t *priv = (app_face_priv_t *)args;
    video_frame_t *frame  = (video_frame_t *)priv->data;
    HI_U64 alg_phy_addr;
    HI_U64 alg_vir_addr;

    framecnt = 0;
    Size0    = 0;
    Size1    = 0;
    while (HI_FALSE == priv->s_bStopSignal) {

        if (priv->cb[APP_ALG_CB_FRAME_GET].func) {
            s32Ret = priv->cb[APP_ALG_CB_FRAME_GET].func(frame, priv->cb[APP_ALG_CB_FRAME_GET].args);
        }

        if (HI_SUCCESS != s32Ret) {
            printf("get failed!\n");
            continue;
        }

        // TO DO: img manager

        if (VIDEO_FRAME_FORMAT_YUV420SP_VU == frame->enPixelFormat
            || VIDEO_FRAME_FORMAT_YUV420SP_UV == frame->enPixelFormat) {
            Size0 = (frame->u32Stride[0]) * (frame->u32Height);
            Size1 = Size0 / 2;
        } else if (VIDEO_FRAME_FORMAT_YUV422SP_UV == frame->enPixelFormat
                   || VIDEO_FRAME_FORMAT_YUV422SP_VU == frame->enPixelFormat) {
            u32Size = (frame->u32Stride[0]) * (frame->u32Height) * 2;
        }

        HI_CHAR *pVirAddr0 = (HI_CHAR *)HI_MPI_SYS_Mmap(frame->u64PhyAddr[0], Size0 + Size1);
        HI_CHAR *pVirAddr1 = pVirAddr0 + Size0; //(HI_CHAR*) HI_MPI_SYS_Mmap(frame->u64PhyAddr[1], Size1);

        alg_vir_addr = (HI_U64)(size_t)pVirAddr0;
        alg_phy_addr = frame->u64PhyAddr[0];

        if (0 == framecnt) {
            s32Ret = HI_MPI_SYS_MmzAlloc_Cached(&stDstData.au64PhyAddr[0],
                                                (HI_VOID **)&stDstData.au64VirAddr[0],
                                                "User",
                                                HI_NULL,
                                                Size0 * 3); // 640*640×3
            if (HI_SUCCESS != s32Ret) {
                HI_MPI_SYS_MmzFree(stSrcData.au64PhyAddr[0], (HI_VOID *)(size_t)stSrcData.au64VirAddr[0]);
                printf("HI_MPI_SYS_MmzAlloc_Cached Failed!");
                return NULL;
            }
        }

        // struct timespec ts1, ts2;
        // clock_gettime(CLOCK_MONOTONIC, &ts1);

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
                printf("s32Ret:%d \n", s32Ret);
                return NULL;
            }
            alg_vir_addr = stDstData.au64VirAddr[0];
            alg_phy_addr = stDstData.au64PhyAddr[0];
        }

        NNIE_IMG_RETINAFACE frame_alg;
        frame_alg.au64PhyAddr[0] = (size_t)alg_phy_addr;
        frame_alg.au64VirAddr[0] = (size_t)alg_vir_addr;
        if (framecnt == 0) {
            saveBMPFile((unsigned char *)(size_t)alg_vir_addr, 640, 640, "1.bmp");
        }
        NNIE_FD_RESULT result;
        memset(&result, 0, sizeof(NNIE_FD_RESULT));
        NNIE_FACE_DETECTOR_GET_NEW(&frame_alg, &result);
        // clock_gettime(CLOCK_MONOTONIC, &ts2);

        /*printf(" cost:%ld ms\n"
            , (ts2.tv_sec*1000 + ts2.tv_nsec/1000000) - (ts1.tv_sec*1000 + ts1.tv_nsec/1000000));*/

        RESULT_BAG result_bag;
        RESULT_BAG result_out;
        memset(&result_bag, 0, sizeof(RESULT_BAG));
        memset(&result_out, 0, sizeof(RESULT_BAG));

        for (i = 0; i < result.fd_num; i++) {
            printf("result rect: %d, %f, %f, %f, %f\n",
                   i,
                   result.fd[i].ul.x,
                   result.fd[i].ul.y,
                   result.fd[i].lr.x,
                   result.fd[i].lr.y);
            // key point
            printf("result lds:  %f, %f\n", result.fd[i].keyPoint[0].x, result.fd[i].keyPoint[0].y);
            printf("result lds:  %f, %f\n", result.fd[i].keyPoint[1].x, result.fd[i].keyPoint[1].y);
            printf("result lds:  %f, %f\n", result.fd[i].keyPoint[2].x, result.fd[i].keyPoint[2].y);
            printf("result lds:  %f, %f\n", result.fd[i].keyPoint[3].x, result.fd[i].keyPoint[3].y);
            printf("result lds:  %f, %f\n", result.fd[i].keyPoint[4].x, result.fd[i].keyPoint[4].y);
            /*\B4\A5\B1ߴ\A6\C0\ED*/
            result.fd[i].ul.x = result.fd[i].ul.x < 0 ? 0 : result.fd[i].ul.x;
            result.fd[i].ul.y = result.fd[i].ul.y < 0 ? 0 : result.fd[i].ul.y;
            result.fd[i].lr.x = result.fd[i].lr.x > 640 ? 640 : result.fd[i].lr.x;
            result.fd[i].lr.y = result.fd[i].lr.y > 640 ? 640 : result.fd[i].lr.y;

            result_bag.obj[i].centerx = (result.fd[i].ul.x + result.fd[i].lr.x) / 2;
            result_bag.obj[i].centery = (result.fd[i].ul.y + result.fd[i].lr.y) / 2;
            result_bag.obj[i].heigth  = result.fd[i].lr.y - result.fd[i].ul.y;
            result_bag.obj[i].width   = result.fd[i].lr.x - result.fd[i].ul.x;
        }

        result_bag.obj_num = result.fd_num;
        if (result_bag.obj_num > 0) {
            framecnt++;
        }
        /* \B8\FA\D7\D9һ\B6\A8Ҫÿ֡\B5\F7\D3\C3 */
        tracker_id(framecnt, &result_bag, &result_out);
        // printf("RESULT num %d\n", result_out.obj_num);

        /*if(result_bag.obj_num > 0)
        {

                for(i = 0; i < result_out.obj_num; i++)
                {
                  printf("RESULT rect: %d, %d, %d, %d, %d\n", result_out.obj[i].ID
        ,result_out.obj[i].width,result_out.obj[i].heigth,result_out.obj[i].centerx,result_out.obj[i].centery);
                }
        }*/

        HI_MPI_SYS_Munmap(pVirAddr0, Size0);
        HI_MPI_SYS_Munmap(pVirAddr0, Size1);

        if (priv->cb[APP_ALG_CB_FRAME_FREE].func) {
            s32Ret = priv->cb[APP_ALG_CB_FRAME_FREE].func(frame, priv->cb[APP_ALG_CB_FRAME_FREE].args);
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
    pthread_create(&priv->s_hMdThread, NULL, hs_fd_task, (void *)priv);
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
    NNIE_FACE_DETECTOR_RELEASE();
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
    if (obj == NULL) {
        return NULL;
    }

    obj->priv        = priv;
    obj->init        = __hs_fd_init;
    obj->destroy     = __hs_fd_destroy;
    obj->regcallback = __hs_fd_regcallback;

    mod->objs = obj;
    return obj;
}
