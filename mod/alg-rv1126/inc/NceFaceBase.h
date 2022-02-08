/*
 * @Author: your name
 * @Date: 2021-12-14 14:51:07
 * @LastEditTime: 2022-01-05 22:06:12
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \NceSimDb\NceSimDb.hpp
 */
#ifndef __NCE_FACE_BASE_HPP__
#define __NCE_FACE_BASE_HPP__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
#include "alg_type.h"
typedef struct Rect
{
	int width;
	int heigth;
	int centerx;
	int centery;
}Rect;

typedef struct NceFdResult_st
{
    int   id;
    int   score;
    int   angle_score;
    int   fake;
    int   landmarks[10];
    Rect  rect;
    img_t img;
}NceFdResult;

typedef struct NceFaceDbInfo_st
{
    char name[32];
    char picdir[32];
    int  man;
    int  age;
    img_t img;
}NceFaceDbInfo;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __NCE_FACE_BASE_HPP__ */