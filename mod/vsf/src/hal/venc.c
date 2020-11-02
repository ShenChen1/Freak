#include "common.h"
#include "inc/hal/venc.h"

vsf_venc_t *__weak VSF_createVenc(int id)
{
    return NULL;
}

int __weak VSF_getVencNum(void)
{
    return -1;
}