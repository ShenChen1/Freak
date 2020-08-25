#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "inc/msgbox.h"
#include "inc/cfg.h"
#include "nnm.h"
#include "log.h"

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize)
{
    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];

    *out = obuf;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main()
{
    int ret;
    nnm_t rep = NULL;
    nnm_t req = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    cfg_load(PROTO_RTSP_CFG_PATH);
    msgbox_init();
    nnm_rep_create(PROTO_BSP_COM_NODE, __rep_recv, &rep);

    // init start
    ret = nnm_req_create(PROTO_BSP_COM_NODE, &req);
    assert(!ret);

    // init end
    ret = nnm_req_destory(req);
    assert(!ret);

    while (1) {
        infof("keep alive");
        sleep(1);
    }

    nnm_rep_destory(rep);
    msgbox_deinit();
    log_deinit();

    return 0;
}