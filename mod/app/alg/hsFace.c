#include "inc/hsFace.h"
#include "algtype.h"
#include "common.h"
#include "hi_comm_ive.h"
#include "inc/basetype.h"
#include "inc/nnie_face_api.h"
#include "inc/sample_comm.h"
#include "log.h"
#include "media.h"
#include "mpi_ive.h"
#include "proto.h"
#include "ufifo.h"

typedef struct {
    ufifo_t *fifo_get;
    ufifo_t *fifo_free;
    pthread_t s_hMdThread;
    HI_BOOL s_bStopSignal;
    uint8_t data[1024]; // img manager
} hs_face_info_t;

static void *hs_fd_task(void *args);
extern int tracker_id(int FrameIndex, RESULT_BAG *result_bag, RESULT_BAG *result_out);

static unsigned int recsize(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        size = rec->size;
    } else {
        media_record_t rec;
        char *p = (char *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        size = rec.size;
    }

    return size;
}

static unsigned int rectag(unsigned char *p1, unsigned int n1, unsigned char *p2)
{
    unsigned int tag;
    unsigned int size = sizeof(media_record_t);

    if (n1 >= size) {
        media_record_t *rec = (media_record_t *)p1;
        tag = rec->tag;
    } else {
        media_record_t rec;
        char *p = (char *)(&rec);
        memcpy(p, p1, n1);
        memcpy(p + n1, p2, size - n1);
        tag = rec.tag;
    }

    return tag;
}

static unsigned int recput(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg)
{
    size_t totalsize     = sizeof(media_record_t) + sizeof(video_frame_t);
    media_record_t rec   = {};
    video_frame_t *frame = arg;
    unsigned int a = 0, l = 0, _n1 = n1;
    unsigned char *p = NULL, *_p1 = p1, *_p2 = p2;

    rec.size = totalsize;
    rec.tag  = (0xdeadbeef << 8 | 1);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    rec.ts = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

    // copy header
    p = (unsigned char *)(&rec);
    a = sizeof(media_record_t);
    l = min(a, _n1);
    memcpy(_p1, p, l);
    memcpy(_p2, p + l, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    // copy data
    p = (unsigned char *)(frame);
    a = sizeof(video_frame_t);
    l = min(a, _n1);
    memcpy(_p1, p, l);
    memcpy(_p2, p + l, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    tracef("frame:%u", frame->u32TimeRef);
    return totalsize;
}

static unsigned int recget(unsigned char *p1, unsigned int n1, unsigned char *p2, void *arg)
{
    media_record_t *rec  = arg;
    video_frame_t *frame = arg;
    unsigned int a = 0, l = 0, _n1 = n1;
    unsigned char *p = NULL, *_p1 = p1, *_p2 = p2;

    // copy header
    p = (unsigned char *)(rec);
    a = sizeof(media_record_t);
    l = min(a, _n1);
    memcpy(p, _p1, l);
    memcpy(p + _n1, _p2, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    // check header
    assert(rec->tag >= (0xdeadbeef << 8));

    // copy data
    p = (unsigned char *)(frame);
    a = sizeof(video_frame_t);
    l = min(a, _n1);
    memcpy(p, _p1, l);
    memcpy(p + l, _p2, a - l);
    _n1 -= l;
    _p1 += l;
    _p2 += a - l;

    tracef("frame:%u", frame->u32TimeRef);
    return sizeof(media_record_t) + sizeof(video_frame_t);
}

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

void *hs_fd_create(char *path)
{
    infof("fd_init");
    hs_face_info_t *priv = malloc(sizeof(hs_face_info_t));
    if (priv == NULL)
        return NULL;
    memset(priv, 0, sizeof(hs_face_info_t));
    float threshold   = 0.6;
    int isLog         = 0;
    char *pcModelName = "/userdata/data/nnie_model/face/mnet_640_inst.wk";
    NNIE_FACE_DETECTOR_INIT(pcModelName, threshold, isLog);

    ufifo_init_t init = {
    .lock = UFIFO_LOCK_NONE,
    .opt  = UFIFO_OPT_ATTACH,
    .attach = { .shared = 0, },
    .hook = { recsize, rectag, recput, recget },
	};
    char name[64];
    snprintf(name, sizeof(name), PROTO_VSF_FRAME_WORK_FIFO "%d-%d", 0, 2);
    ufifo_open(name, &init, &priv->fifo_get);

    snprintf(name, sizeof(name), PROTO_VSF_FRAME_FREE_FIFO "%d-%d", 0, 2);
    ufifo_open(name, &init, &priv->fifo_free);
    pthread_create(&priv->s_hMdThread, NULL, hs_fd_task, (void *)priv);

    return priv;
}

int hs_fd_destroy(void *args)
{
    hs_face_info_t *priv = (hs_face_info_t *)args;

    priv->s_bStopSignal = HI_TRUE;
    return pthread_join(priv->s_hMdThread, NULL);
    NNIE_FACE_DETECTOR_RELEASE();
}

static void *hs_fd_task(void *args)
{
    IVE_CSC_CTRL_S stCscCtrl;
    IVE_DST_IMAGE_S stDstData;
    IVE_SRC_IMAGE_S stSrcData;
    IVE_HANDLE hIveHandle;

    int i, framecnt, Size0, Size1, u32Size = 0;
    HI_S32 s32Ret        = 0;
    HI_BOOL bInstant     = HI_TRUE;
    hs_face_info_t *priv = (hs_face_info_t *)args;
    video_frame_t *frame = (video_frame_t *)priv->data;
    HI_U64 alg_phy_addr;
    HI_U64 alg_vir_addr;

    framecnt = 0;
    Size0    = 0;
    Size1    = 0;
    while (HI_FALSE == priv->s_bStopSignal) {

        memset(frame, 0, sizeof(video_frame_t));
        if (priv->fifo_get) {

            ufifo_get_block(priv->fifo_get, frame, sizeof(priv->data));
        }

        if (frame->u64PrivateData == 0) {
            printf("get failed!\n");
            continue;
        }

        // to do img manager

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

        size_t totalsize = sizeof(media_record_t) + sizeof(video_frame_t);
        ufifo_put(priv->fifo_free, frame, totalsize);
    }
    HI_MPI_SYS_MmzFree(stDstData.au64PhyAddr[0], (HI_VOID *)(size_t)stDstData.au64VirAddr[0]);
    return 0;
}
