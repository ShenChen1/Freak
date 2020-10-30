#include "common.h"
#include "inc/hal/vi.h"

vsf_vi_t *__weak VSF_createVi(int id)
{
    return NULL;
}

int __weak VSF_getViNum(void)
{
    return -1;
}