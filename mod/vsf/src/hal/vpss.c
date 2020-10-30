#include "common.h"
#include "inc/hal/vpss.h"

vsf_vpss_t *__weak VSF_createVpss(int id)
{
    return NULL;
}

int __weak VSF_getVpssNum(void)
{
    return -1;
}