#include <stdio.h>
#include <errno.h>
#include "inc/dummy.h"
#include "inc/msgbox.h"
#include "inc/cfg.h"

int msgbox_dummy(uint8_t chn, uint8_t action, void *in, size_t isize, void *out, size_t *osize)
{
    int ret;
    dummy_t *obj = NULL;
    proto_bsp_dummy_t *dummy_in = in;
    proto_bsp_dummy_t *dummy_out = out;

    obj = createDummy(chn);
    if (obj == NULL) {
        return -EINVAL;
    }

    if (action == PROTO_ACTION_SET) {
        ret = obj->set(obj, dummy_in->value);
        *osize = 0;
        *cfg_get_member(dummy) = *dummy_in;
    } else {
        ret = obj->get(obj, &dummy_out->value);
        *osize = sizeof(proto_bsp_dummy_t);
    }

    return ret;
}