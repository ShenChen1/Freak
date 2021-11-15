#include "inc/hal/venc.h"
#include "common.h"
#include "inc/sdk_cfg.h"
#include "log.h"
#include "media.h"

typedef struct {
    sdk_venc_info_t *info;
    pthread_t hq_thread;
    int quit;
    int seq;
    vsf_stream_cb_t cb;
} vsf_venc_priv_t;

typedef struct {
    int num;
    vsf_venc_t **objs;
} vsf_venc_mod_t;

static vsf_venc_mod_t s_mod;

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __transfor_encode_format(uint32_t pt)
{
    int i;
    const int convert[][2] = {
        { RK_CODEC_TYPE_H264, VIDEO_STREAM_TYPE_H264 },
        { RK_CODEC_TYPE_H265, VIDEO_STREAM_TYPE_H265 },
        { RK_CODEC_TYPE_JPEG, VIDEO_STREAM_TYPE_JPEG },
        { RK_CODEC_TYPE_MJPEG, VIDEO_STREAM_TYPE_MJPEG },
    };

    for (i = 0; i < ARRAY_SIZE(convert); i++) {
        if (convert[i][0] == pt) {
            return convert[i][1];
        }
    }

    assert(i == ARRAY_SIZE(convert));
    return -1;
}

static void __transfor_stream_info(MEDIA_BUFFER mb, video_stream_t *dst,vsf_venc_priv_t *priv)
{
    dst->u32Seq       = priv->seq++;
    dst->u32PackCount = 1;
    dst->pstPack      = realloc(dst->pstPack, dst->u32PackCount * sizeof(video_stream_pack_t));
    assert(dst->pstPack);

    dst->pstPack[0].u64PhyAddr  = 0;
    dst->pstPack[0].pu8Addr     = RK_MPI_MB_GetPtr(mb);
    dst->pstPack[0].u32Len      = RK_MPI_MB_GetSize(mb);
    dst->pstPack[0].u32Offset   = 0;
    dst->pstPack[0].u64PTS      = RK_MPI_MB_GetTimestamp(mb);
    dst->pstPack[0].u32PackType = (uint32_t)RK_MPI_MB_GetFlag(mb); 
    printf("dst pts %lld\n",dst->pstPack[0].u64PTS);
}

/*void video_packet_cb(MEDIA_BUFFER mb)
{
    static int packet_cnt = 0;
    packet_cnt++;
    printf("#Get packet-%d, size %zu\n", packet_cnt, RK_MPI_MB_GetSize(mb));
    vsf_venc_t *obj       = s_mod.objs[0];
    vsf_venc_priv_t *priv = obj->priv;
    int s32Ret            = 0;
    video_stream_t stream = { NULL };
    if (priv->cb.func)
    {
        __transfor_stream_info(mb, &stream,priv);
        stream.enType = __transfor_encode_format(priv->info->enType);
        s32Ret        = priv->cb.func(&stream, priv->cb.args);
        if (0 != s32Ret) {
            errorf("proc stream failed!");
        }
    }
    RK_MPI_MB_ReleaseBuffer(mb);
    return;
}*/

static void *GetMediaBuffer(void *arg) {
   /* printf("#Start %s hq_thread, arg:%p\n", __func__, arg);
    char out_path[128];
    sprintf(out_path, "%s/output.h265", g_save_path);
    FILE *save_file = fopen(out_path, "w");
    if (!save_file)
        printf("ERROR: Open %s failed!\n", out_path);*/
    vsf_venc_priv_t *priv   = arg;
    int s32Ret            = 0;
    video_stream_t stream = { NULL };
    MEDIA_BUFFER mb = NULL;
    while (!priv->quit) {
        mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_VENC, priv->info->ChnId, -1);
        if (!mb) {
            printf("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
            break;
        }
        if (priv->cb.func)
        {
            __transfor_stream_info(mb, &stream,priv);
            stream.enType = __transfor_encode_format(priv->info->enType);
            s32Ret        = priv->cb.func(&stream, priv->cb.args);
            if (0 != s32Ret) {
                errorf("proc stream failed!");
            }
    }
        //    printf("Get packet:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
        //           "timestamp:%lld\n",
        //           RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb),
        //           RK_MPI_MB_GetSize(mb),
        //           RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb),
        //           RK_MPI_MB_GetTimestamp(mb));

        //if (save_file)
            //fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), save_file);
        RK_MPI_MB_ReleaseBuffer(mb);
    }

    //if (save_file)
        //fclose(save_file);

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

static int __venc_init(vsf_venc_t *self)
{
    int s32Ret;
    vsf_venc_t *obj       = self;
    vsf_venc_priv_t *priv = obj->priv;
    VENC_CHN_ATTR_S venc_chn_attr;
    memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));

    venc_chn_attr.stVencAttr.enType = priv->info->enType;
    switch (venc_chn_attr.stVencAttr.enType) {
    case RK_CODEC_TYPE_H265:
        venc_chn_attr.stRcAttr.enRcMode             = priv->info->enRcMode;
        venc_chn_attr.stRcAttr.stH265Cbr.u32Gop     = priv->info->Gop;
        venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate = priv->info->PicWidth * priv->info->PicHeight;
        // frame rate: in 30/1, out 30/1.
        venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateDen = priv->info->SrcFrameRateDen;
        venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateNum = priv->info->SrcFrameRateNum;
        venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateDen  = priv->info->DstFrameRateDen;
        venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateNum  = priv->info->DstFrameRateNum;
        break;
    case RK_CODEC_TYPE_H264:
    default:
        venc_chn_attr.stRcAttr.enRcMode             = priv->info->enRcMode;
        venc_chn_attr.stRcAttr.stH264Cbr.u32Gop     = priv->info->Gop;
        venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = priv->info->PicWidth * priv->info->PicHeight;
        // frame rate: in 30/1, out 30/1.
        venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = priv->info->DstFrameRateDen;
        venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = priv->info->DstFrameRateNum;
        venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen  = priv->info->SrcFrameRateDen;
        venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum  = priv->info->SrcFrameRateNum;
        break;
    }
    venc_chn_attr.stVencAttr.imageType    = priv->info->imageType;
    venc_chn_attr.stVencAttr.u32PicWidth  = priv->info->PicWidth;
    venc_chn_attr.stVencAttr.u32PicHeight = priv->info->PicHeight;
    venc_chn_attr.stVencAttr.u32VirWidth  = (priv->info->PicWidth + 15) & (~15);
    venc_chn_attr.stVencAttr.u32VirHeight = priv->info->PicHeight;//(priv->info->PicHeight + 15) & (~15);
    venc_chn_attr.stVencAttr.u32Profile   = priv->info->Profile;

    VENC_GOP_ATTR_S stGopModeAttr;
    stGopModeAttr.enGopMode = VENC_GOPMODE_SMARTP;
    stGopModeAttr.s32IPQpDelta = 3;
    stGopModeAttr.s32ViQpDelta = 3;
    stGopModeAttr.u32BgInterval = 300;
    stGopModeAttr.u32GopSize = 30;
    RK_MPI_VENC_SetGopMode(0, &stGopModeAttr);

    s32Ret = RK_MPI_VENC_CreateChn(priv->info->ChnId, &venc_chn_attr);
    if (s32Ret) {
        printf("ERROR: create VENC[0] error! ret=%d\n", s32Ret);
        return 0;
    }

    //两种处理方案，回调和线程，暂时采用自己做线程方案
    /*MPP_CHN_S stEncChn;
    stEncChn.enModId = RK_ID_VENC;
    stEncChn.s32DevId = priv->info->DevId;
    stEncChn.s32ChnId = priv->info->ChnId;
    s32Ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, video_packet_cb);
    if (s32Ret) {
        printf("ERROR: register output callback for VENC[0] error! ret=%d\n", s32Ret);
        return 0;
    }*/
    //RK_MPI_VI_StartStream(priv->info->DevId, priv->info->ChnId);
    MPP_CHN_S stSrcChn;
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32ChnId = priv->info->ViChnId;
    MPP_CHN_S stDestChn;
    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32ChnId = priv->info->ChnId;
    s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    pthread_create(&priv->hq_thread, NULL, GetMediaBuffer, priv);
    return s32Ret;
}

static int __venc_destroy(vsf_venc_t *self)
{
    vsf_venc_mod_t *mod   = &s_mod;
    vsf_venc_t *obj       = self;
    vsf_venc_priv_t *priv = obj->priv;


    priv->quit = 1;
    pthread_join(priv->hq_thread, NULL);
    MPP_CHN_S stSrcChn;
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32ChnId = priv->info->ViChnId;
    MPP_CHN_S stDestChn;
    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32ChnId = priv->info->ChnId;
    RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    RK_MPI_VENC_DestroyChn(priv->info->ChnId);
    mod->objs[priv->info->ChnId] = NULL;
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
    priv->info = sdk_cfg_get_member(astVencInfo[id]);

    obj = malloc(sizeof(vsf_venc_t));
    assert(obj);
    memset(obj, 0, sizeof(vsf_venc_t));
    obj->priv        = priv;
    obj->init        = __venc_init;
    obj->destroy     = __venc_destroy;
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