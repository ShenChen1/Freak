#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <nanomsg/nn.h>
#include "nnm.h"

void *nnm_allocmsg(size_t size)
{
    return nn_allocmsg(size, 0);
}

void *nnm_reallocmsg(void *msg, size_t size)
{
    return nn_reallocmsg(msg, size);
}

int nnm_free(void *msg)
{
    return nnm_free(msg);
}
