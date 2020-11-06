#include "common.h"
#include "log.h"
#include "media.h"
#include "inc/hal/venc.h"
#include "inc/sdk_cfg.h"

typedef struct {
    int virtid;
    int phyid;
    sdk_venc_info_t *info;
    vsf_stream_cb_t cb;
} vsf_venc_priv_t;

typedef struct {
    int num;
    vsf_venc_t **objs;
    pthread_t thread;
} vsf_venc_mod_t;

static vsf_venc_mod_t s_mod;

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __transfor_encode_format(PAYLOAD_TYPE_E pt)
{
    int i;
    const int convert[][2] = {
        {PT_H264, VIDEO_ENCODE_TYPE_H264},
        {PT_H265, VIDEO_ENCODE_TYPE_H265},
        {PT_JPEG, VIDEO_ENCODE_TYPE_JPEG},
        {PT_MJPEG, VIDEO_ENCODE_TYPE_MJPEG},
    };

    for (i = 0; i < ARRAY_SIZE(convert); i++) {
        if (convert[i][0] == pt) {
            return convert[i][1];
        }
    }

    assert(i == ARRAY_SIZE(convert));
    return VIDEO_ENCODE_TYPE_H264;
}

static void __transfor_stream_format(VENC_STREAM_S *src, video_stream_t *dst)
{
    uint i;

    dst->u32Seq       = src->u32Seq;
    dst->u32PackCount = src->u32PackCount;
    dst->pstPack      = realloc(dst->pstPack, dst->u32PackCount * sizeof(video_pack_t));
    assert(dst->pstPack);

    for (i = 0; i < src->u32PackCount; i++) {
        dst->pstPack[i].u64PhyAddr  = src->pstPack[i].u64PhyAddr;
        dst->pstPack[i].pu8Addr     = src->pstPack[i].pu8Addr;
        dst->pstPack[i].u32Len      = src->pstPack[i].u32Len;
        dst->pstPack[i].u32Offset   = src->pstPack[i].u32Offset;
        dst->pstPack[i].u64PTS      = src->pstPack[i].u64PTS;
        dst->pstPack[i].u32PackType = (uint32_t)src->pstPack[i].DataType.enH264EType;
    }
}

static HI_VOID *SAMPLE_COMM_VENC_GetVencStreamProc(HI_VOID *p)
{
    HI_S32 i, s32Ret, maxfd = -1;
    VENC_CHN_STATUS_S stStat;
    VENC_STREAM_S stStream;
    VENC_CHN_ATTR_S stVencChnAttr;
    HI_U32 u32PictureCnt[VENC_MAX_CHN_NUM] = { 0 };
    HI_S32 VencFd[VENC_MAX_CHN_NUM];
    PAYLOAD_TYPE_E enPayLoadType[VENC_MAX_CHN_NUM];
    vsf_venc_mod_t *mod   = p;
    vsf_venc_priv_t *priv = NULL;
    video_encode_type_e encode = VIDEO_ENCODE_TYPE_H264;
    video_stream_t stream = { NULL };

    /******************************************
     step 1:  check & prepare venc-fd
    ******************************************/
    for (i = 0; i < mod->num; i++) {
        priv = mod->objs[i]->priv;

        s32Ret = HI_MPI_VENC_GetChnAttr(priv->phyid, &stVencChnAttr);
        if (s32Ret != HI_SUCCESS) {
            errorf("HI_MPI_VENC_GetChnAttr chn[%d] failed with %#x!", priv->phyid, s32Ret);
            return NULL;
        }
        enPayLoadType[i] = stVencChnAttr.stVencAttr.enType;

        /* Set Venc Fd. */
        VencFd[i] = HI_MPI_VENC_GetFd(i);
        if (VencFd[i] < 0) {
            errorf("HI_MPI_VENC_GetFd failed with %#x!", VencFd[i]);
            return NULL;
        }
        maxfd = max(VencFd[i], maxfd);
    }

    /******************************************
     step 2:  Start to get streams of each channel.
    ******************************************/
    while (1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        for (i = 0; i < mod->num; i++) {
            FD_SET(VencFd[i], &read_fds);
        }

        struct timeval TimeoutVal;
        TimeoutVal.tv_sec  = 2;
        TimeoutVal.tv_usec = 0;
        s32Ret = select(maxfd + 1, &read_fds, NULL, NULL, &TimeoutVal);
        if (s32Ret < 0) {
            errorf("select failed!");
            break;
        } else if (s32Ret == 0) {
            errorf("get venc stream time out, exit thread");
            continue;
        }

        for (i = 0; i < mod->num; i++) {
            if (FD_ISSET(VencFd[i], &read_fds)) {
                /*******************************************************
                 step 2.1 : query how many packs in one-frame stream.
                *******************************************************/
                s32Ret = HI_MPI_VENC_QueryStatus(i, &stStat);
                if (HI_SUCCESS != s32Ret) {
                    errorf("HI_MPI_VENC_QueryStatus chn[%d] failed with %#x!\n", i, s32Ret);
                    break;
                }
                /*******************************************************
                step 2.2 :suggest to check both u32CurPacks and u32LeftStreamFrames at the same time,for example:
                if(0 == stStat.u32CurPacks || 0 == stStat.u32LeftStreamFrames) {
                    errorf("NOTE: Current  frame is NULL!");
                    continue;
                }
                *******************************************************/
                if (0 == stStat.u32CurPacks) {
                    errorf("NOTE: Current frame is NULL!");
                    continue;
                }
                /*******************************************************
                 step 2.3 : malloc corresponding number of pack nodes.
                *******************************************************/
                stStream.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S) * stStat.u32CurPacks);
                if (NULL == stStream.pstPack) {
                    errorf("malloc stream pack failed!");
                    break;
                }
                /*******************************************************
                 step 2.4 : call mpi to get one-frame stream
                *******************************************************/
                stStream.u32PackCount = stStat.u32CurPacks;
                memset(&stStream, 0, sizeof(stStream));
                s32Ret = HI_MPI_VENC_GetStream(i, &stStream, HI_TRUE);
                if (HI_SUCCESS != s32Ret) {
                    free(stStream.pstPack);
                    stStream.pstPack = NULL;
                    errorf("HI_MPI_VENC_GetStream failed with %#x!\n", s32Ret);
                    break;
                }
                /*******************************************************
                 step 2.5 : process frame
                *******************************************************/
                priv = mod->objs[i]->priv;
                if (priv->cb.func) {
                    __transfor_stream_format(&stStream, &stream);
                    encode = __transfor_encode_format(enPayLoadType[i]);
                    s32Ret = priv->cb.func(i, encode, &stream, priv->cb.args);
                    if (HI_SUCCESS != s32Ret) {
                        free(stStream.pstPack);
                        stStream.pstPack = NULL;
                        errorf("proc stream failed!");
                        break;
                    }
                }
                /*******************************************************
                 step 2.6 : release stream
                *******************************************************/
                s32Ret = HI_MPI_VENC_ReleaseStream(i, &stStream);
                if (HI_SUCCESS != s32Ret) {
                    errorf("HI_MPI_VENC_ReleaseStream failed!");
                    free(stStream.pstPack);
                    stStream.pstPack = NULL;
                    break;
                }
                /*******************************************************
                 step 2.7 : free pack nodes
                *******************************************************/
                free(stStream.pstPack);
                stStream.pstPack = NULL;
                u32PictureCnt[i]++;
            }
        }
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __venc_init(vsf_venc_t *self)
{
    int s32Ret;
    vsf_venc_t *obj       = self;
    vsf_venc_priv_t *priv = obj->priv;

    VENC_GOP_ATTR_S stGopAttr;
    s32Ret = SAMPLE_COMM_VENC_GetGopAttr(priv->info->enGopMode, &stGopAttr);
    if (HI_SUCCESS != s32Ret) {
        errorf("Venc Get GopAttr for %#x!", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VENC_Start(priv->info->VencChn,
                                    priv->info->enPayLoad,
                                    priv->info->enSize,
                                    priv->info->enRcMode,
                                    priv->info->u32Profile,
                                    priv->info->bRcnRefShareBuf,
                                    &stGopAttr);
    if (HI_SUCCESS != s32Ret) {
        errorf("Venc Start failed for %#x!", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_VPSS_Bind_VENC(priv->info->VpssGrp, priv->info->VpssChn, priv->info->VencChn);
    if (HI_SUCCESS != s32Ret) {
        errorf("Venc Get GopAttr failed for %#x!", s32Ret);
        SAMPLE_COMM_VENC_Stop(priv->info->VencChn);
        return s32Ret;
    }

    return s32Ret;
}

static int __venc_destroy(vsf_venc_t *self)
{
    vsf_venc_mod_t *mod   = &s_mod;
    vsf_venc_t *obj       = self;
    vsf_venc_priv_t *priv = obj->priv;

    SAMPLE_COMM_VPSS_UnBind_VENC(priv->info->VpssGrp, priv->info->VpssChn, priv->info->VencChn);
    SAMPLE_COMM_VENC_Stop(priv->info->VencChn);
    mod->objs[priv->virtid] = NULL;
    free(priv);
    free(obj);

    return 0;
}

static int __venc_regcallback(vsf_venc_t *self, vsf_stream_cb_t *cb)
{
    vsf_venc_t *obj       = self;
    vsf_venc_priv_t *priv = obj->priv;

    if (cb == NULL) {
        priv->cb.args = NULL;
        priv->cb.func = NULL;
    } else {
        priv->cb.args = cb->args;
        priv->cb.func = cb->func;
    }

    return 0;
}

vsf_venc_t *VSF_createVenc(int id)
{
    vsf_venc_mod_t *mod   = &s_mod;
    vsf_venc_t *obj       = NULL;
    vsf_venc_priv_t *priv = NULL;

    if (id >= mod->num) {
        return NULL;
    }

    obj = mod->objs[id];
    if (obj) {
        return obj;
    }

    priv = malloc(sizeof(vsf_venc_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(vsf_venc_priv_t));
    priv->virtid = id;
    priv->phyid  = *sdk_cfg_get_member(as32VencId[id]);
    priv->info   = sdk_cfg_get_member(astVencInfo[id]);

    obj = malloc(sizeof(vsf_venc_priv_t));
    if (obj == NULL) {
        return NULL;
    }

    obj->priv    = priv;
    obj->init    = __venc_init;
    obj->destroy = __venc_destroy;
    obj->regcallback = __venc_regcallback;

    mod->objs[id] = obj;
    return obj;
}

int VSF_getVencNum(void)
{
    vsf_venc_mod_t *mod = &s_mod;
    return mod->num;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static void __attribute__((constructor(VSF_VENC_PRIORITY))) sdk_venc_constructor()
{
    int i;
    vsf_venc_mod_t *mod = &s_mod;

    mod->num  = *sdk_cfg_get_member(s32VencNum);
    mod->objs = calloc(mod->num, sizeof(vsf_venc_t *));
    assert(mod->objs);

    for (i = 0; i < mod->num; i++) {
        vsf_venc_t *obj = VSF_createVenc(i);
        assert(!obj->init(obj));
    }

    pthread_create(&mod->thread, NULL, SAMPLE_COMM_VENC_GetVencStreamProc, mod);
}

static void __attribute__((destructor(VSF_VENC_PRIORITY))) sdk_venc_destructor()
{
    int i;
    vsf_venc_mod_t *mod = &s_mod;

    for (i = 0; i < mod->num; i++) {
        vsf_venc_t *obj = VSF_createVenc(i);
        assert(!obj->destroy(obj));
    }

    free(mod->objs);
    mod->num = 0;
}