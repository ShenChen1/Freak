#include "common.h"
#include "inc/hal/sys.h"

vsf_sys_t *__weak VSF_createSys(int id)
{
    return NULL;
}

int __weak VSF_getSysNum(void)
{
    return -1;
}