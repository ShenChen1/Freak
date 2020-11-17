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
        { PT_H264, VIDEO_ENCODE_TYPE_H264 },
        { PT_H265, VIDEO_ENCODE_TYPE_H265 },
        { PT_JPEG, VIDEO_ENCODE_TYPE_JPEG },
        { PT_MJPEG, VIDEO_ENCODE_TYPE_MJPEG },
    };

    for (i = 0; i < ARRAY_SIZE(convert); i++) {
        if (convert[i][0] == pt) {
            return convert[i][1];
        }
    }

    assert(i == ARRAY_SIZE(convert));
    return -1;
}

static void __transfor_stream_info(VENC_STREAM_S *src, video_stream_t *dst)
{
    int i;

    dst->u32Seq       = src->u32Seq;
    dst->u32PackCount = src->u32PackCount;
    dst->pstPack      = realloc(dst->pstPack, dst->u32PackCount * sizeof(video_stream_pack_t));
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

        struct timeval timeoutVal;
        timeoutVal.tv_sec  = 2;
        timeoutVal.tv_usec = 0;
        s32Ret = select(maxfd + 1, &read_fds, NULL, NULL, &timeoutVal);
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
                    errorf("HI_MPI_VENC_QueryStatus chn[%d] failed with %#x!", i, s32Ret);
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
                memset(&stStream, 0, sizeof(stStream));
                stStream.pstPack = (VENC_PACK_S *)malloc(sizeof(VENC_PACK_S) * stStat.u32CurPacks);
                if (NULL == stStream.pstPack) {
                    errorf("malloc stream pack failed!");
                    break;
                }
                /*******************************************************
                 step 2.4 : call mpi to get one-frame stream
                *******************************************************/
                stStream.u32PackCount = stStat.u32CurPacks;
                s32Ret = HI_MPI_VENC_GetStream(i, &stStream, HI_TRUE);
                if (HI_SUCCESS != s32Ret) {
                    free(stStream.pstPack);
                    stStream.pstPack = NULL;
                    errorf("HI_MPI_VENC_GetStream failed with %#x!", s32Ret);
                    break;
                }
                /*******************************************************
                 step 2.5 : process frame
                *******************************************************/
                priv = mod->objs[i]->priv;
                if (priv->cb.func) {
                    __transfor_stream_info(&stStream, &stream);
                    video_encode_type_e encode = VIDEO_ENCODE_TYPE_H264;
                    encode = __transfor_encode_format(enPayLoadType[i]);
                    s32Ret = priv->cb.func(i, encode, &stream, priv->cb.args);
                    if (HI_SUCCESS != s32Ret) {
                        errorf("proc stream failed!");
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

HI_S32 SAMPLE_COMM_VENC_Create(VENC_CHN VencChn,
                               PAYLOAD_TYPE_E enType,
                               PIC_SIZE_E enSize,
                               SAMPLE_RC_E enRcMode,
                               HI_U32 u32Profile,
                               HI_BOOL bRcnRefShareBuf,
                               VENC_GOP_ATTR_S *pstGopAttr)
{
    HI_S32 s32Ret;
    SIZE_S stPicSize;
    VENC_CHN_ATTR_S stVencChnAttr;
    VENC_ATTR_JPEG_S stJpegAttr;
    HI_U32 u32StatTime;
    HI_U32 u32Gop = 30;

    s32Ret = SAMPLE_COMM_SYS_GetPicSize(enSize, &stPicSize);
    if (HI_SUCCESS != s32Ret) {
        errorf("Get picture size failed!");
        return HI_FAILURE;
    }

    SAMPLE_VI_CONFIG_S stViConfig;
    HI_U32 u32FrameRate;
    SAMPLE_COMM_VI_GetSensorInfo(&stViConfig);
    if (SAMPLE_SNS_TYPE_BUTT == stViConfig.astViInfo[0].stSnsInfo.enSnsType) {
        errorf("Not set SENSOR%d_TYPE !", 0);
        return HI_FALSE;
    }
    s32Ret = SAMPLE_COMM_VI_GetFrameRateBySensor(stViConfig.astViInfo[0].stSnsInfo.enSnsType, &u32FrameRate);
    if (HI_SUCCESS != s32Ret) {
        errorf("SAMPLE_COMM_VI_GetFrameRateBySensor failed!");
        return s32Ret;
    }

    /******************************************
     step 1:  Create Venc Channel
    ******************************************/
    stVencChnAttr.stVencAttr.enType          = enType;
    stVencChnAttr.stVencAttr.u32MaxPicWidth  = stPicSize.u32Width;
    stVencChnAttr.stVencAttr.u32MaxPicHeight = stPicSize.u32Height;
    stVencChnAttr.stVencAttr.u32PicWidth     = stPicSize.u32Width;                           /*the picture width*/
    stVencChnAttr.stVencAttr.u32PicHeight    = stPicSize.u32Height;                          /*the picture height*/
    stVencChnAttr.stVencAttr.u32BufSize      = stPicSize.u32Width * stPicSize.u32Height * 2; /*stream buffer size*/
    stVencChnAttr.stVencAttr.u32Profile      = u32Profile;
    stVencChnAttr.stVencAttr.bByFrame        = HI_TRUE; /*get stream mode is slice mode or frame mode?*/

    if (VENC_GOPMODE_SMARTP == pstGopAttr->enGopMode) {
        u32StatTime = pstGopAttr->stSmartP.u32BgInterval / u32Gop;
    } else {
        u32StatTime = 1;
    }

    switch (enType) {
    case PT_H265: {
        if (SAMPLE_RC_CBR == enRcMode) {
            VENC_H265_CBR_S stH265Cbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
            stH265Cbr.u32Gop                = u32Gop;
            stH265Cbr.u32StatTime           = u32StatTime;  /* stream rate statics time(s) */
            stH265Cbr.u32SrcFrameRate       = u32FrameRate; /* input (vi) frame rate */
            stH265Cbr.fr32DstFrameRate      = u32FrameRate; /* target frame rate */
            switch (enSize) {
            case PIC_720P:
                stH265Cbr.u32BitRate = 1024 * 2 + 1024 * u32FrameRate / 30;
                break;
            case PIC_1080P:
                stH265Cbr.u32BitRate = 1024 * 2 + 2048 * u32FrameRate / 30;
                break;
            case PIC_2592x1944:
                stH265Cbr.u32BitRate = 1024 * 3 + 3072 * u32FrameRate / 30;
                break;
            case PIC_3840x2160:
                stH265Cbr.u32BitRate = 1024 * 5 + 5120 * u32FrameRate / 30;
                break;
            case PIC_4000x3000:
                stH265Cbr.u32BitRate = 1024 * 10 + 5120 * u32FrameRate / 30;
                break;
            case PIC_7680x4320:
                stH265Cbr.u32BitRate = 1024 * 20 + 5120 * u32FrameRate / 30;
                break;
            default:
                stH265Cbr.u32BitRate = 1024 * 15 + 2048 * u32FrameRate / 30;
                break;
            }
            memcpy(&stVencChnAttr.stRcAttr.stH265Cbr, &stH265Cbr, sizeof(VENC_H265_CBR_S));
        } else if (SAMPLE_RC_FIXQP == enRcMode) {
            VENC_H265_FIXQP_S stH265FixQp;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265FIXQP;
            stH265FixQp.u32Gop              = 30;
            stH265FixQp.u32SrcFrameRate     = u32FrameRate;
            stH265FixQp.fr32DstFrameRate    = u32FrameRate;
            stH265FixQp.u32IQp              = 25;
            stH265FixQp.u32PQp              = 30;
            stH265FixQp.u32BQp              = 32;
            memcpy(&stVencChnAttr.stRcAttr.stH265FixQp, &stH265FixQp, sizeof(VENC_H265_FIXQP_S));
        } else if (SAMPLE_RC_VBR == enRcMode) {
            VENC_H265_VBR_S stH265Vbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
            stH265Vbr.u32Gop                = u32Gop;
            stH265Vbr.u32StatTime           = u32StatTime;
            stH265Vbr.u32SrcFrameRate       = u32FrameRate;
            stH265Vbr.fr32DstFrameRate      = u32FrameRate;
            switch (enSize) {
            case PIC_720P:
                stH265Vbr.u32MaxBitRate = 1024 * 2 + 1024 * u32FrameRate / 30;
                break;
            case PIC_1080P:
                stH265Vbr.u32MaxBitRate = 1024 * 2 + 2048 * u32FrameRate / 30;
                break;
            case PIC_2592x1944:
                stH265Vbr.u32MaxBitRate = 1024 * 3 + 3072 * u32FrameRate / 30;
                break;
            case PIC_3840x2160:
                stH265Vbr.u32MaxBitRate = 1024 * 5 + 5120 * u32FrameRate / 30;
                break;
            case PIC_4000x3000:
                stH265Vbr.u32MaxBitRate = 1024 * 10 + 5120 * u32FrameRate / 30;
                break;
            case PIC_7680x4320:
                stH265Vbr.u32MaxBitRate = 1024 * 20 + 5120 * u32FrameRate / 30;
                break;
            default:
                stH265Vbr.u32MaxBitRate = 1024 * 15 + 2048 * u32FrameRate / 30;
                break;
            }
            memcpy(&stVencChnAttr.stRcAttr.stH265Vbr, &stH265Vbr, sizeof(VENC_H265_VBR_S));
        } else if (SAMPLE_RC_AVBR == enRcMode) {
            VENC_H265_AVBR_S stH265AVbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265AVBR;
            stH265AVbr.u32Gop               = u32Gop;
            stH265AVbr.u32StatTime          = u32StatTime;
            stH265AVbr.u32SrcFrameRate      = u32FrameRate;
            stH265AVbr.fr32DstFrameRate     = u32FrameRate;
            switch (enSize) {
            case PIC_720P:
                stH265AVbr.u32MaxBitRate = 1024 * 2 + 1024 * u32FrameRate / 30;
                break;
            case PIC_1080P:
                stH265AVbr.u32MaxBitRate = 1024 * 2 + 2048 * u32FrameRate / 30;
                break;
            case PIC_2592x1944:
                stH265AVbr.u32MaxBitRate = 1024 * 3 + 3072 * u32FrameRate / 30;
                break;
            case PIC_3840x2160:
                stH265AVbr.u32MaxBitRate = 1024 * 5 + 5120 * u32FrameRate / 30;
                break;
            case PIC_4000x3000:
                stH265AVbr.u32MaxBitRate = 1024 * 10 + 5120 * u32FrameRate / 30;
                break;
            case PIC_7680x4320:
                stH265AVbr.u32MaxBitRate = 1024 * 20 + 5120 * u32FrameRate / 30;
                break;
            default:
                stH265AVbr.u32MaxBitRate = 1024 * 15 + 2048 * u32FrameRate / 30;
                break;
            }
            memcpy(&stVencChnAttr.stRcAttr.stH265AVbr, &stH265AVbr, sizeof(VENC_H265_AVBR_S));
        } else if (SAMPLE_RC_QVBR == enRcMode) {
            VENC_H265_QVBR_S stH265QVbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265QVBR;
            stH265QVbr.u32Gop               = u32Gop;
            stH265QVbr.u32StatTime          = u32StatTime;
            stH265QVbr.u32SrcFrameRate      = u32FrameRate;
            stH265QVbr.fr32DstFrameRate     = u32FrameRate;
            switch (enSize) {
            case PIC_720P:
                stH265QVbr.u32TargetBitRate = 1024 * 2 + 1024 * u32FrameRate / 30;
                break;
            case PIC_1080P:
                stH265QVbr.u32TargetBitRate = 1024 * 2 + 2048 * u32FrameRate / 30;
                break;
            case PIC_2592x1944:
                stH265QVbr.u32TargetBitRate = 1024 * 3 + 3072 * u32FrameRate / 30;
                break;
            case PIC_3840x2160:
                stH265QVbr.u32TargetBitRate = 1024 * 5 + 5120 * u32FrameRate / 30;
                break;
            case PIC_4000x3000:
                stH265QVbr.u32TargetBitRate = 1024 * 10 + 5120 * u32FrameRate / 30;
                break;
            case PIC_7680x4320:
                stH265QVbr.u32TargetBitRate = 1024 * 20 + 5120 * u32FrameRate / 30;
                break;
            default:
                stH265QVbr.u32TargetBitRate = 1024 * 15 + 2048 * u32FrameRate / 30;
                break;
            }
            memcpy(&stVencChnAttr.stRcAttr.stH265QVbr, &stH265QVbr, sizeof(VENC_H265_QVBR_S));
        } else if (SAMPLE_RC_CVBR == enRcMode) {
            VENC_H265_CVBR_S stH265CVbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CVBR;
            stH265CVbr.u32Gop               = u32Gop;
            stH265CVbr.u32StatTime          = u32StatTime;
            stH265CVbr.u32SrcFrameRate      = u32FrameRate;
            stH265CVbr.fr32DstFrameRate     = u32FrameRate;
            stH265CVbr.u32LongTermStatTime  = 1;
            stH265CVbr.u32ShortTermStatTime = u32StatTime;
            switch (enSize) {
            case PIC_720P:
                stH265CVbr.u32MaxBitRate         = 1024 * 3 + 1024 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMaxBitrate = 1024 * 2 + 1024 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMinBitrate = 512;
                break;
            case PIC_1080P:
                stH265CVbr.u32MaxBitRate         = 1024 * 2 + 2048 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMaxBitrate = 1024 * 2 + 2048 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMinBitrate = 1024;
                break;
            case PIC_2592x1944:
                stH265CVbr.u32MaxBitRate         = 1024 * 4 + 3072 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMaxBitrate = 1024 * 3 + 3072 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMinBitrate = 1024 * 2;
                break;
            case PIC_3840x2160:
                stH265CVbr.u32MaxBitRate         = 1024 * 8 + 5120 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMaxBitrate = 1024 * 5 + 5120 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMinBitrate = 1024 * 3;
                break;
            case PIC_4000x3000:
                stH265CVbr.u32MaxBitRate         = 1024 * 12 + 5120 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMaxBitrate = 1024 * 10 + 5120 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMinBitrate = 1024 * 4;
                break;
            case PIC_7680x4320:
                stH265CVbr.u32MaxBitRate         = 1024 * 24 + 5120 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMaxBitrate = 1024 * 20 + 5120 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMinBitrate = 1024 * 6;
                break;
            default:
                stH265CVbr.u32MaxBitRate         = 1024 * 24 + 5120 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMaxBitrate = 1024 * 15 + 2048 * u32FrameRate / 30;
                stH265CVbr.u32LongTermMinBitrate = 1024 * 5;
                break;
            }
            memcpy(&stVencChnAttr.stRcAttr.stH265CVbr, &stH265CVbr, sizeof(VENC_H265_CVBR_S));
        } else if (SAMPLE_RC_QPMAP == enRcMode) {
            VENC_H265_QPMAP_S stH265QpMap;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265QPMAP;
            stH265QpMap.u32Gop              = u32Gop;
            stH265QpMap.u32StatTime         = u32StatTime;
            stH265QpMap.u32SrcFrameRate     = u32FrameRate;
            stH265QpMap.fr32DstFrameRate    = u32FrameRate;
            stH265QpMap.enQpMapMode         = VENC_RC_QPMAP_MODE_MEANQP;
            memcpy(&stVencChnAttr.stRcAttr.stH265QpMap, &stH265QpMap, sizeof(VENC_H265_QPMAP_S));
        } else {
            errorf("%s,%d,enRcMode(%d) not support", __FUNCTION__, __LINE__, enRcMode);
            return HI_FAILURE;
        }
        stVencChnAttr.stVencAttr.stAttrH265e.bRcnRefShareBuf = bRcnRefShareBuf;
    } break;
    case PT_H264: {
        if (SAMPLE_RC_CBR == enRcMode) {
            VENC_H264_CBR_S stH264Cbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
            stH264Cbr.u32Gop                = u32Gop;       /*the interval of IFrame*/
            stH264Cbr.u32StatTime           = u32StatTime;  /* stream rate statics time(s) */
            stH264Cbr.u32SrcFrameRate       = u32FrameRate; /* input (vi) frame rate */
            stH264Cbr.fr32DstFrameRate      = u32FrameRate; /* target frame rate */
            switch (enSize) {
            case PIC_720P:
                stH264Cbr.u32BitRate = 1024 * 3 + 1024 * u32FrameRate / 30;
                break;
            case PIC_1080P:
                stH264Cbr.u32BitRate = 1024 * 2 + 2048 * u32FrameRate / 30;
                break;
            case PIC_2592x1944:
                stH264Cbr.u32BitRate = 1024 * 4 + 3072 * u32FrameRate / 30;
                break;
            case PIC_3840x2160:
                stH264Cbr.u32BitRate = 1024 * 8 + 5120 * u32FrameRate / 30;
                break;
            case PIC_4000x3000:
                stH264Cbr.u32BitRate = 1024 * 12 + 5120 * u32FrameRate / 30;
                break;
            case PIC_7680x4320:
                stH264Cbr.u32BitRate = 1024 * 24 + 5120 * u32FrameRate / 30;
                break;
            default:
                stH264Cbr.u32BitRate = 1024 * 24 + 5120 * u32FrameRate / 30;
                break;
            }

            memcpy(&stVencChnAttr.stRcAttr.stH264Cbr, &stH264Cbr, sizeof(VENC_H264_CBR_S));
        } else if (SAMPLE_RC_FIXQP == enRcMode) {
            VENC_H264_FIXQP_S stH264FixQp;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264FIXQP;
            stH264FixQp.u32Gop              = 30;
            stH264FixQp.u32SrcFrameRate     = u32FrameRate;
            stH264FixQp.fr32DstFrameRate    = u32FrameRate;
            stH264FixQp.u32IQp              = 25;
            stH264FixQp.u32PQp              = 30;
            stH264FixQp.u32BQp              = 32;
            memcpy(&stVencChnAttr.stRcAttr.stH264FixQp, &stH264FixQp, sizeof(VENC_H264_FIXQP_S));
        } else if (SAMPLE_RC_VBR == enRcMode) {
            VENC_H264_VBR_S stH264Vbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
            stH264Vbr.u32Gop                = u32Gop;
            stH264Vbr.u32StatTime           = u32StatTime;
            stH264Vbr.u32SrcFrameRate       = u32FrameRate;
            stH264Vbr.fr32DstFrameRate      = u32FrameRate;
            switch (enSize) {
            case PIC_360P:
                stH264Vbr.u32MaxBitRate = 1024 * 2 + 1024 * u32FrameRate / 30;
                break;
            case PIC_720P:
                stH264Vbr.u32MaxBitRate = 1024 * 2 + 1024 * u32FrameRate / 30;
                break;
            case PIC_1080P:
                stH264Vbr.u32MaxBitRate = 1024 * 2 + 2048 * u32FrameRate / 30;
                break;
            case PIC_2592x1944:
                stH264Vbr.u32MaxBitRate = 1024 * 3 + 3072 * u32FrameRate / 30;
                break;
            case PIC_3840x2160:
                stH264Vbr.u32MaxBitRate = 1024 * 5 + 5120 * u32FrameRate / 30;
                break;
            case PIC_4000x3000:
                stH264Vbr.u32MaxBitRate = 1024 * 10 + 5120 * u32FrameRate / 30;
                break;
            case PIC_7680x4320:
                stH264Vbr.u32MaxBitRate = 1024 * 20 + 5120 * u32FrameRate / 30;
                break;
            default:
                stH264Vbr.u32MaxBitRate = 1024 * 15 + 2048 * u32FrameRate / 30;
                break;
            }
            memcpy(&stVencChnAttr.stRcAttr.stH264Vbr, &stH264Vbr, sizeof(VENC_H264_VBR_S));
        } else if (SAMPLE_RC_AVBR == enRcMode) {
            VENC_H264_VBR_S stH264AVbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264AVBR;
            stH264AVbr.u32Gop               = u32Gop;
            stH264AVbr.u32StatTime          = u32StatTime;
            stH264AVbr.u32SrcFrameRate      = u32FrameRate;
            stH264AVbr.fr32DstFrameRate     = u32FrameRate;
            switch (enSize) {
            case PIC_360P:
                stH264AVbr.u32MaxBitRate = 1024 * 2 + 1024 * u32FrameRate / 30;
                break;
            case PIC_720P:
                stH264AVbr.u32MaxBitRate = 1024 * 2 + 1024 * u32FrameRate / 30;
                break;
            case PIC_1080P:
                stH264AVbr.u32MaxBitRate = 1024 * 2 + 2048 * u32FrameRate / 30;
                break;
            case PIC_2592x1944:
                stH264AVbr.u32MaxBitRate = 1024 * 3 + 3072 * u32FrameRate / 30;
                break;
            case PIC_3840x2160:
                stH264AVbr.u32MaxBitRate = 1024 * 5 + 5120 * u32FrameRate / 30;
                break;
            case PIC_4000x3000:
                stH264AVbr.u32MaxBitRate = 1024 * 10 + 5120 * u32FrameRate / 30;
                break;
            case PIC_7680x4320:
                stH264AVbr.u32MaxBitRate = 1024 * 20 + 5120 * u32FrameRate / 30;
                break;
            default:
                stH264AVbr.u32MaxBitRate = 1024 * 15 + 2048 * u32FrameRate / 30;
                break;
            }
            memcpy(&stVencChnAttr.stRcAttr.stH264AVbr, &stH264AVbr, sizeof(VENC_H264_AVBR_S));
        } else if (SAMPLE_RC_QVBR == enRcMode) {
            VENC_H264_QVBR_S stH264QVbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264QVBR;
            stH264QVbr.u32Gop               = u32Gop;
            stH264QVbr.u32StatTime          = u32StatTime;
            stH264QVbr.u32SrcFrameRate      = u32FrameRate;
            stH264QVbr.fr32DstFrameRate     = u32FrameRate;
            switch (enSize) {
            case PIC_360P:
                stH264QVbr.u32TargetBitRate = 1024 * 2 + 1024 * u32FrameRate / 30;
                break;
            case PIC_720P:
                stH264QVbr.u32TargetBitRate = 1024 * 2 + 1024 * u32FrameRate / 30;
                break;
            case PIC_1080P:
                stH264QVbr.u32TargetBitRate = 1024 * 2 + 2048 * u32FrameRate / 30;
                break;
            case PIC_2592x1944:
                stH264QVbr.u32TargetBitRate = 1024 * 3 + 3072 * u32FrameRate / 30;
                break;
            case PIC_3840x2160:
                stH264QVbr.u32TargetBitRate = 1024 * 5 + 5120 * u32FrameRate / 30;
                break;
            case PIC_4000x3000:
                stH264QVbr.u32TargetBitRate = 1024 * 10 + 5120 * u32FrameRate / 30;
                break;
            case PIC_7680x4320:
                stH264QVbr.u32TargetBitRate = 1024 * 20 + 5120 * u32FrameRate / 30;
                break;
            default:
                stH264QVbr.u32TargetBitRate = 1024 * 15 + 2048 * u32FrameRate / 30;
                break;
            }
            memcpy(&stVencChnAttr.stRcAttr.stH264QVbr, &stH264QVbr, sizeof(VENC_H264_QVBR_S));
        } else if (SAMPLE_RC_CVBR == enRcMode) {
            VENC_H264_CVBR_S stH264CVbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CVBR;
            stH264CVbr.u32Gop               = u32Gop;
            stH264CVbr.u32StatTime          = u32StatTime;
            stH264CVbr.u32SrcFrameRate      = u32FrameRate;
            stH264CVbr.fr32DstFrameRate     = u32FrameRate;
            stH264CVbr.u32LongTermStatTime  = 1;
            stH264CVbr.u32ShortTermStatTime = u32StatTime;
            switch (enSize) {
            case PIC_720P:
                stH264CVbr.u32MaxBitRate         = 1024 * 3 + 1024 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMaxBitrate = 1024 * 2 + 1024 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMinBitrate = 512;
                break;
            case PIC_1080P:
                stH264CVbr.u32MaxBitRate         = 1024 * 2 + 2048 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMaxBitrate = 1024 * 2 + 2048 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMinBitrate = 1024;
                break;
            case PIC_2592x1944:
                stH264CVbr.u32MaxBitRate         = 1024 * 4 + 3072 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMaxBitrate = 1024 * 3 + 3072 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMinBitrate = 1024 * 2;
                break;
            case PIC_3840x2160:
                stH264CVbr.u32MaxBitRate         = 1024 * 8 + 5120 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMaxBitrate = 1024 * 5 + 5120 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMinBitrate = 1024 * 3;
                break;
            case PIC_4000x3000:
                stH264CVbr.u32MaxBitRate         = 1024 * 12 + 5120 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMaxBitrate = 1024 * 10 + 5120 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMinBitrate = 1024 * 4;
                break;
            case PIC_7680x4320:
                stH264CVbr.u32MaxBitRate         = 1024 * 24 + 5120 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMaxBitrate = 1024 * 20 + 5120 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMinBitrate = 1024 * 6;
                break;
            default:
                stH264CVbr.u32MaxBitRate         = 1024 * 24 + 5120 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMaxBitrate = 1024 * 15 + 2048 * u32FrameRate / 30;
                stH264CVbr.u32LongTermMinBitrate = 1024 * 5;
                break;
            }
            memcpy(&stVencChnAttr.stRcAttr.stH264CVbr, &stH264CVbr, sizeof(VENC_H264_CVBR_S));
        } else if (SAMPLE_RC_QPMAP == enRcMode) {
            VENC_H264_QPMAP_S stH264QpMap;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264QPMAP;
            stH264QpMap.u32Gop              = u32Gop;
            stH264QpMap.u32StatTime         = u32StatTime;
            stH264QpMap.u32SrcFrameRate     = u32FrameRate;
            stH264QpMap.fr32DstFrameRate    = u32FrameRate;
            memcpy(&stVencChnAttr.stRcAttr.stH264QpMap, &stH264QpMap, sizeof(VENC_H264_QPMAP_S));
        } else {
            errorf("%s,%d,enRcMode(%d) not support", __FUNCTION__, __LINE__, enRcMode);
            return HI_FAILURE;
        }
        stVencChnAttr.stVencAttr.stAttrH264e.bRcnRefShareBuf = bRcnRefShareBuf;
    } break;
    case PT_MJPEG: {
        if (SAMPLE_RC_FIXQP == enRcMode) {
            VENC_MJPEG_FIXQP_S stMjpegeFixQp;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGFIXQP;
            stMjpegeFixQp.u32Qfactor        = 95;
            stMjpegeFixQp.u32SrcFrameRate   = u32FrameRate;
            stMjpegeFixQp.fr32DstFrameRate  = u32FrameRate;

            memcpy(&stVencChnAttr.stRcAttr.stMjpegFixQp, &stMjpegeFixQp, sizeof(VENC_MJPEG_FIXQP_S));
        } else if (SAMPLE_RC_CBR == enRcMode) {
            VENC_MJPEG_CBR_S stMjpegeCbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGCBR;
            stMjpegeCbr.u32StatTime         = u32StatTime;
            stMjpegeCbr.u32SrcFrameRate     = u32FrameRate;
            stMjpegeCbr.fr32DstFrameRate    = u32FrameRate;
            switch (enSize) {
            case PIC_360P:
                stMjpegeCbr.u32BitRate = 1024 * 3 + 1024 * u32FrameRate / 30;
                break;
            case PIC_720P:
                stMjpegeCbr.u32BitRate = 1024 * 5 + 1024 * u32FrameRate / 30;
                break;
            case PIC_1080P:
                stMjpegeCbr.u32BitRate = 1024 * 8 + 2048 * u32FrameRate / 30;
                break;
            case PIC_2592x1944:
                stMjpegeCbr.u32BitRate = 1024 * 20 + 3072 * u32FrameRate / 30;
                break;
            case PIC_3840x2160:
                stMjpegeCbr.u32BitRate = 1024 * 25 + 5120 * u32FrameRate / 30;
                break;
            case PIC_4000x3000:
                stMjpegeCbr.u32BitRate = 1024 * 30 + 5120 * u32FrameRate / 30;
                break;
            case PIC_7680x4320:
                stMjpegeCbr.u32BitRate = 1024 * 40 + 5120 * u32FrameRate / 30;
                break;
            default:
                stMjpegeCbr.u32BitRate = 1024 * 20 + 2048 * u32FrameRate / 30;
                break;
            }

            memcpy(&stVencChnAttr.stRcAttr.stMjpegCbr, &stMjpegeCbr, sizeof(VENC_MJPEG_CBR_S));
        } else if ((SAMPLE_RC_VBR == enRcMode) || (SAMPLE_RC_AVBR == enRcMode) || (SAMPLE_RC_QVBR == enRcMode)
                   || (SAMPLE_RC_CVBR == enRcMode)) {
            VENC_MJPEG_VBR_S stMjpegVbr;

            if (SAMPLE_RC_AVBR == enRcMode) {
                errorf("Mjpege not support AVBR, so change rcmode to VBR!");
            }

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGVBR;
            stMjpegVbr.u32StatTime          = u32StatTime;
            stMjpegVbr.u32SrcFrameRate      = u32FrameRate;
            stMjpegVbr.fr32DstFrameRate     = 5;

            switch (enSize) {
            case PIC_360P:
                stMjpegVbr.u32MaxBitRate = 1024 * 3 + 1024 * u32FrameRate / 30;
                break;
            case PIC_720P:
                stMjpegVbr.u32MaxBitRate = 1024 * 5 + 1024 * u32FrameRate / 30;
                break;
            case PIC_1080P:
                stMjpegVbr.u32MaxBitRate = 1024 * 8 + 2048 * u32FrameRate / 30;
                break;
            case PIC_2592x1944:
                stMjpegVbr.u32MaxBitRate = 1024 * 20 + 3072 * u32FrameRate / 30;
                break;
            case PIC_3840x2160:
                stMjpegVbr.u32MaxBitRate = 1024 * 25 + 5120 * u32FrameRate / 30;
                break;
            case PIC_4000x3000:
                stMjpegVbr.u32MaxBitRate = 1024 * 30 + 5120 * u32FrameRate / 30;
                break;
            case PIC_7680x4320:
                stMjpegVbr.u32MaxBitRate = 1024 * 40 + 5120 * u32FrameRate / 30;
                break;
            default:
                stMjpegVbr.u32MaxBitRate = 1024 * 20 + 2048 * u32FrameRate / 30;
                break;
            }

            memcpy(&stVencChnAttr.stRcAttr.stMjpegVbr, &stMjpegVbr, sizeof(VENC_MJPEG_VBR_S));
        } else {
            errorf("cann't support other mode(%d) in this version!", enRcMode);
            return HI_FAILURE;
        }
    } break;

    case PT_JPEG:
        stJpegAttr.bSupportDCF                  = HI_FALSE;
        stJpegAttr.stMPFCfg.u8LargeThumbNailNum = 0;
        stJpegAttr.enReceiveMode                = VENC_PIC_RECEIVE_SINGLE;
        memcpy(&stVencChnAttr.stVencAttr.stAttrJpege, &stJpegAttr, sizeof(VENC_ATTR_JPEG_S));
        break;
    default:
        errorf("cann't support this enType (%d) in this version!", enType);
        return HI_ERR_VENC_NOT_SUPPORT;
    }

    if (PT_MJPEG == enType || PT_JPEG == enType) {
        stVencChnAttr.stGopAttr.enGopMode              = VENC_GOPMODE_NORMALP;
        stVencChnAttr.stGopAttr.stNormalP.s32IPQpDelta = 0;
    } else {
        memcpy(&stVencChnAttr.stGopAttr, pstGopAttr, sizeof(VENC_GOP_ATTR_S));
        if ((VENC_GOPMODE_BIPREDB == pstGopAttr->enGopMode) && (PT_H264 == enType)) {
            if (0 == stVencChnAttr.stVencAttr.u32Profile) {
                stVencChnAttr.stVencAttr.u32Profile = 1;
                errorf("H.264 base profile not support BIPREDB, so change profile to main profile!");
            }
        }

        if ((VENC_RC_MODE_H264QPMAP == stVencChnAttr.stRcAttr.enRcMode) ||
            (VENC_RC_MODE_H265QPMAP == stVencChnAttr.stRcAttr.enRcMode)) {
            if (VENC_GOPMODE_ADVSMARTP == pstGopAttr->enGopMode) {
                stVencChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
                errorf("advsmartp not support QPMAP, so change gopmode to smartp!");
            }
        }
    }

    s32Ret = HI_MPI_VENC_CreateChn(VencChn, &stVencChnAttr);
    if (HI_SUCCESS != s32Ret) {
        errorf("HI_MPI_VENC_CreateChn [%d] faild with %#x! ===", VencChn, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
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

    /******************************************
     step 1:  Create Encode Chnl
    ******************************************/
    s32Ret = SAMPLE_COMM_VENC_Create(priv->info->VencChn,
                                     priv->info->enPayLoad,
                                     priv->info->enSize,
                                     priv->info->enRcMode,
                                     priv->info->u32Profile,
                                     priv->info->bRcnRefShareBuf,
                                     &stGopAttr);
    if (HI_SUCCESS != s32Ret) {
        errorf("SAMPLE_COMM_VENC_Create faild with%#x!", s32Ret);
        return HI_FAILURE;
    }

    /******************************************
     step 2:  Start Recv Venc Pictures
    ******************************************/
    VENC_RECV_PIC_PARAM_S stRecvParam;
    stRecvParam.s32RecvPicNum = -1;
    s32Ret = HI_MPI_VENC_StartRecvFrame(priv->info->VencChn, &stRecvParam);
    if (HI_SUCCESS != s32Ret) {
        errorf("HI_MPI_VENC_StartRecvPic faild with%#x!", s32Ret);
        return HI_FAILURE;
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