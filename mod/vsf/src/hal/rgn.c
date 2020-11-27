#include "common.h"
#include "inc/hal/rgn.h"

vsf_rgn_t *__weak VSF_createRgn(int id)
{
    return NULL;
}

int __weak VSF_getRgnNum(void)
{
    return -1;
}