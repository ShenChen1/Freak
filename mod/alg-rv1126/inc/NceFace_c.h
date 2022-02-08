/*
 * @Author: your name
 * @Date: 2021-08-31 15:24:33
 * @LastEditTime: 2022-01-20 14:59:00
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \NceAlg\nce_alg\alg\nce_alg_c.h
 */
#ifndef __NCE_FACE_C_H__
#define __NCE_FACE_C_H__
#include "alg_type.h"
#include "NceFaceBase.h"
#ifdef __cplusplus
#if __cplusplus

extern "C" {
#endif
#endif /* __cplusplus */
#define PARAM_NUM_MAX 8
#define IMG_NUM_MAX 8
/*typedef struct nce_alg_c_machine
{
    NCE_PTR  pPriv;
} */

typedef NCE_S32  (*NceFaceDetect_c)(NCE_PTR pPriv, img_t *pimg, NceFdResult **facelist, int *num);

typedef NCE_S32 (*NceFaceRecognize_c)(NCE_PTR pPriv, img_t *pimg,int *landmarks,float thres);

typedef NCE_S32 (*NceFaceDbInsert_c)(NCE_PTR pPriv, NceFaceDbInfo *infolist,int num);

typedef NCE_S32 (*NceFaceGetDbData_c)(NCE_PTR pPriv, int *idlist,int num,NceFaceDbInfo **infolist);


typedef struct nce_alg_c_machine
{
    NCE_PTR               pPriv;
    NceFaceDetect_c       face_det;
    NceFaceRecognize_c    face_rec;
    NceFaceDbInsert_c     facedb_insert;
    NceFaceGetDbData_c    facedb_getdata;
} nce_face_bucket;

NCE_S32 nce_face_bucket_init(nce_face_bucket *bucket,const char *fdpath, const char *frpath);
NCE_S32 nce_face_bucket_deinit(nce_face_bucket *bucket);

NCE_S32 nce_img_crop_soft_rgb888_pln(img_t* srcimg,img_t *dstimg,Rect *rect);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __NCE_FACE_C_H__ */