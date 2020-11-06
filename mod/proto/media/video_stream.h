#ifndef __VIDEO_STREAM_H__
#define __VIDEO_STREAM_H__

/* the nalu type of H264E */
typedef enum {
    VIDEO_H264E_NALU_BSLICE   = 0, /*B SLICE types*/
    VIDEO_H264E_NALU_PSLICE   = 1, /*P SLICE types*/
    VIDEO_H264E_NALU_ISLICE   = 2, /*I SLICE types*/
    VIDEO_H264E_NALU_IDRSLICE = 5, /*IDR SLICE types*/
    VIDEO_H264E_NALU_SEI      = 6, /*SEI types*/
    VIDEO_H264E_NALU_SPS      = 7, /*SPS types*/
    VIDEO_H264E_NALU_PPS      = 8, /*PPS types*/
    VIDEO_H264E_NALU_BUTT
} video_h264e_nalu_type_e;

/* the data type of VENC */
typedef union {
    video_h264e_nalu_type_e enH264EType; /* R; H264E NALU types */
} video_data_type_u;

/* Defines a stream packet */
typedef struct {
    uint64_t u64PhyAddr; /* R; the physics address of stream */
    uint8_t *pu8Addr;    /* R; the virtual address of stream */
    uint32_t u32Len;     /* R; the length of stream */
    uint32_t u32Offset;  /* R; the offset of pu8Addr */

    uint64_t u64PTS;      /* R; PTS */
    uint32_t u32PackType; /* R; the pack type */
} video_pack_t;

/* Defines the features of an stream */
typedef struct {
    video_pack_t *pstPack; /* R; stream pack attribute */
    uint32_t u32PackCount; /* R; the pack number of one frame stream */
    uint32_t u32Seq;       /* R; the list number of stream */
} video_stream_t;

typedef enum {
    VIDEO_ENCODE_TYPE_H264,
    VIDEO_ENCODE_TYPE_H265,
    VIDEO_ENCODE_TYPE_JPEG,
    VIDEO_ENCODE_TYPE_MJPEG,
} video_encode_type_e;

#endif /* __VIDEO_STREAM_H__ */