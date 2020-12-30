#include "common.h"
#include "log.h"
#include "nnm.h"
#include "inc/msgbox.h"
#include "inc/cfg.h"
#include "vsf/frame_mgr.h"
#include "vsf/stream_mgr.h"
#include "vsf/osd_mgr.h"

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize, void *arg)
{
    *out = arg;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main()
{
    nnm_t rep = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    cfg_load(PROTO_VSF_CFG_PATH);
    msgbox_init();

    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = { __rep_recv, obuf };
    nnm_rep_create(PROTO_VSF_COM_NODE, &init, &rep);

    int i, total;
    //vsf_frame_mgr_t *frame = vsf_createFrameMgr();
    //total = frame->num(frame);
    /*for (i = 0; i < total; i++) {
        proto_vsf_frame_cfg_t cfg = {.id = i};
        // get cfg
        frame->get(frame, &cfg);
        // set cfg
        frame->set(frame, &cfg);
    }
    frame->destroy(frame);*/

    vsf_stream_mgr_t *stream = vsf_createStreamMgr();
    total = stream->num(stream);
    for (i = 0; i < total; i++) {
        proto_vsf_stream_cfg_t cfg = {.id = i};
        // get cfg
        stream->get(stream, &cfg);
        // set cfg
        stream->set(stream, &cfg);
    }
    stream->destroy(stream);

    vsf_osd_mgr_t *osd = vsf_createOsdMgr();
    total = osd->num(osd);
    for (i = 0; i < total; i++) {
        proto_vsf_osd_cfg_t cfg = {.id = i};
        // get cfg
        osd->get(osd, &cfg);
        // set cfg
        osd->set(osd, &cfg);
    }
    osd->destroy(osd);

    while (1) {
        infof("keep alive");
        sleep(1);
    }

    nnm_rep_destory(rep);
    msgbox_deinit();
    log_deinit();

    return 0;
}
