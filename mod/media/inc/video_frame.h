#ifndef __VIDEO_FRAME_H__
#define __VIDEO_FRAME_H__

typedef enum {
   VIDEO_FRAME_FORMAT_YUV422I_UYVY = 0x0000,
    /**< YUV 422 Interleaved format - UYVY. */
   VIDEO_FRAME_FORMAT_YUV422I_YUYV,
    /**< YUV 422 Interleaved format - YUYV. */
   VIDEO_FRAME_FORMAT_YUV422I_YVYU,
    /**< YUV 422 Interleaved format - YVYU. */
   VIDEO_FRAME_FORMAT_YUV422I_VYUY,
    /**< YUV 422 Interleaved format - VYUY. */
   VIDEO_FRAME_FORMAT_YUV422SP_UV,
    /**< YUV 422 Semi-Planar - Y separate, UV interleaved. */
   VIDEO_FRAME_FORMAT_YUV422SP_VU,
    /**< YUV 422 Semi-Planar - Y separate, VU interleaved. */
   VIDEO_FRAME_FORMAT_YUV422P,
    /**< YUV 422 Planar - Y, U and V separate. */
   VIDEO_FRAME_FORMAT_YUV420SP_UV,
    /**< YUV 420 Semi-Planar - Y separate, UV interleaved. */
   VIDEO_FRAME_FORMAT_YUV420SP_VU,
    /**< YUV 420 Semi-Planar - Y separate, VU interleaved. */

    /**< YUV 420 Planar - Y, U and V separate. */
   VIDEO_FRAME_FORMAT_YUV420P_YUV,
   VIDEO_FRAME_FORMAT_YUV420P_YVU,

   VIDEO_FRAME_FORMAT_NV12 = VIDEO_FRAME_FORMAT_YUV420SP_UV,
   VIDEO_FRAME_FORMAT_I420 = VIDEO_FRAME_FORMAT_YUV420P_YUV,
   VIDEO_FRAME_FORMAT_YV12 = VIDEO_FRAME_FORMAT_YUV420P_YVU,

   VIDEO_FRAME_FORMAT_Y,
    /**<only y */

   VIDEO_FRAME_FORMAT_YUV444P,
    /**< YUV 444 Planar - Y, U and V separate. */

   VIDEO_FRAME_FORMAT_YUV444I,
    /**< YUV 444 interleaved - YUVYUV... */

   VIDEO_FRAME_FORMAT_RGB16_565 = 0x1000,
    /**< RGB565 16-bit - 5-bits R, 6-bits G, 5-bits B. */
   VIDEO_FRAME_FORMAT_ARGB16_1555,
    /**< ARGB1555 16-bit - 5-bits R, 5-bits G, 5-bits B, 1-bit Alpha (MSB). */
   VIDEO_FRAME_FORMAT_RGBA16_5551,
    /**< RGBA5551 16-bit - 5-bits R, 5-bits G, 5-bits B, 1-bit Alpha (LSB). */
   VIDEO_FRAME_FORMAT_ARGB16_4444,
    /**< ARGB4444 16-bit - 4-bits R, 4-bits G, 4-bits B, 4-bit Alpha (MSB). */
   VIDEO_FRAME_FORMAT_RGBA16_4444,
    /**< RGBA4444 16-bit - 4-bits R, 4-bits G, 4-bits B, 4-bit Alpha (LSB). */
   VIDEO_FRAME_FORMAT_ARGB24_6666,
    /**< ARGB6666 24-bit - 6-bits R, 6-bits G, 6-bits B, 6-bit Alpha (MSB). */
   VIDEO_FRAME_FORMAT_RGBA24_6666,
    /**< RGBA6666 24-bit - 6-bits R, 6-bits G, 6-bits B, 6-bit Alpha (LSB). */
   VIDEO_FRAME_FORMAT_RGB24_888,

    /**< RGB24 24-bit - 8-bits R, 8-bits G, 8-bits B. */
   VIDEO_FRAME_FORMAT_ARGB32_8888,
    /**< ARGB32 32-bit - 8-bits R, 8-bits G, 8-bits B, 8-bit Alpha (MSB). */
   VIDEO_FRAME_FORMAT_RGBA32_8888,
    /**< RGBA32 32-bit - 8-bits R, 8-bits G, 8-bits B, 8-bit Alpha (LSB). */
   VIDEO_FRAME_FORMAT_BGR16_565,
    /**< BGR565 16-bit - 5-bits B, 6-bits G, 5-bits R. */
   VIDEO_FRAME_FORMAT_ABGR16_1555,
    /**< ABGR1555 16-bit - 5-bits B, 5-bits G, 5-bits R, 1-bit Alpha (MSB). */
   VIDEO_FRAME_FORMAT_ABGR16_4444,
    /**< ABGR4444 16-bit - 4-bits B, 4-bits G, 4-bits R, 4-bit Alpha (MSB). */
   VIDEO_FRAME_FORMAT_BGRA16_5551,
    /**< BGRA5551 16-bit - 5-bits B, 5-bits G, 5-bits R, 1-bit Alpha (LSB). */
   VIDEO_FRAME_FORMAT_BGRA16_4444,
    /**< BGRA4444 16-bit - 4-bits B, 4-bits G, 4-bits R, 4-bit Alpha (LSB). */
   VIDEO_FRAME_FORMAT_ABGR24_6666,
    /**< ABGR6666 24-bit - 6-bits B, 6-bits G, 6-bits R, 6-bit Alpha (MSB). */
   VIDEO_FRAME_FORMAT_BGR24_888,
    /**< BGR888 24-bit - 8-bits B, 8-bits G, 8-bits R. */

   VIDEO_FRAME_FORMAT_ABGR32_8888,
    /**< ABGR8888 32-bit - 8-bits B, 8-bits G, 8-bits R, 8-bit Alpha (MSB). */
   VIDEO_FRAME_FORMAT_BGRA24_6666,
    /**< BGRA6666 24-bit - 6-bits B, 6-bits G, 6-bits R, 6-bit Alpha (LSB). */
   VIDEO_FRAME_FORMAT_BGRA32_8888,
    /**< BGRA8888 32-bit - 8-bits B, 8-bits G, 8-bits R, 8-bit Alpha (LSB). */

   VIDEO_FRAME_FORMAT_BITMAP8 = 0x2000,
    /**< BITMAP 8bpp. */
   VIDEO_FRAME_FORMAT_BITMAP4_LOWER,
    /**< BITMAP 4bpp lower address in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP4_UPPER,
    /**< BITMAP 4bpp upper address in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP2_OFFSET0,
    /**< BITMAP 2bpp offset 0 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP2_OFFSET1,
    /**< BITMAP 2bpp offset 1 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP2_OFFSET2,
    /**< BITMAP 2bpp offset 2 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP2_OFFSET3,
    /**< BITMAP 2bpp offset 3 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_OFFSET0,
    /**< BITMAP 1bpp offset 0 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_OFFSET1,
    /**< BITMAP 1bpp offset 1 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_OFFSET2,
    /**< BITMAP 1bpp offset 2 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_OFFSET3,
    /**< BITMAP 1bpp offset 3 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_OFFSET4,
    /**< BITMAP 1bpp offset 4 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_OFFSET5,
    /**< BITMAP 1bpp offset 5 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_OFFSET6,
    /**< BITMAP 1bpp offset 6 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_OFFSET7,
    /**< BITMAP 1bpp offset 7 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP8_BGRA32,
    /**< BITMAP 8bpp BGRA32. */

   VIDEO_FRAME_FORMAT_BITMAP4_BGRA32_LOWER,
    /**< BITMAP 4bpp BGRA32 lower address in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP4_BGRA32_UPPER,
    /**< BITMAP 4bpp BGRA32 upper address in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP2_BGRA32_OFFSET0,
    /**< BITMAP 2bpp BGRA32 offset 0 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP2_BGRA32_OFFSET1,
    /**< BITMAP 2bpp BGRA32 offset 1 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP2_BGRA32_OFFSET2,
    /**< BITMAP 2bpp BGRA32 offset 2 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP2_BGRA32_OFFSET3,
    /**< BITMAP 2bpp BGRA32 offset 3 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_BGRA32_OFFSET0,
    /**< BITMAP 1bpp BGRA32 offset 0 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_BGRA32_OFFSET1,
    /**< BITMAP 1bpp BGRA32 offset 1 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_BGRA32_OFFSET2,
    /**< BITMAP 1bpp BGRA32 offset 2 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_BGRA32_OFFSET3,
    /**< BITMAP 1bpp BGRA32 offset 3 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_BGRA32_OFFSET4,
    /**< BITMAP 1bpp BGRA32 offset 4 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_BGRA32_OFFSET5,
    /**< BITMAP 1bpp BGRA32 offset 5 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_BGRA32_OFFSET6,
    /**< BITMAP 1bpp BGRA32 offset 6 in CLUT. */
   VIDEO_FRAME_FORMAT_BITMAP1_BGRA32_OFFSET7,
    /**< BITMAP 1bpp BGRA32 offset 7 in CLUT. */

   VIDEO_FRAME_FORMAT_BAYER_RAW = 0x3000,
    /**< Bayer pattern. */

   VIDEO_FRAME_FORMAT_RAW_VBI,
    /**< Raw VBI data. */

   VIDEO_FRAME_FORMAT_RAW,
    /**< Raw data - Format not interpreted. */

   VIDEO_FRAME_FORMAT_MISC,
    /**< For future purpose. */
   VIDEO_FRAME_FORMAT_INVALID
    /**< Invalid data format. Could be used to initialize variables. */
} video_frame_format_e;

typedef struct {
    uint32_t u32Width;
    uint32_t u32Height;
    uint32_t size;
    video_frame_format_e enPixelFormat;
    uint32_t u32HeaderStride[3];
    uint32_t u32Stride[3];
    uint32_t u32ExtStride[3];

    uint64_t u64HeaderPhyAddr[3];
    uint64_t u64HeaderVirAddr[3];
    uint64_t u64PhyAddr[3];
    uint64_t u64VirAddr[3];
    uint64_t u64ExtPhyAddr[3];
    uint64_t u64ExtVirAddr[3];

    int16_t s16OffsetTop;    /* top offset of show area */
    int16_t s16OffsetBottom; /* bottom offset of show area */
    int16_t s16OffsetLeft;   /* left offset of show area */
    int16_t s16OffsetRight;  /* right offset of show area */

    uint32_t u32TimeRef;
    uint64_t u64PTS;

    uint64_t u64PrivateData;
    void *ptr;
} video_frame_t;

#endif /* __VIDEO_FRAME_H__ */