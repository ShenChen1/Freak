#include "common.h"
#include "nnm.h"
#include "log.h"
#include "inc/msgbox.h"
#include "inc/cfg.h"

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize, void *arg)
{
    *out = arg;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main()
{
    int ret, i;
    nnm_t rep = NULL;
    nnm_t req = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    cfg_load(PROTO_VSF_CFG_PATH);
    msgbox_init();

    static uint8_t ibuf[PROTO_PACKAGE_MAXSIZE];
    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = { __rep_recv, obuf };
    nnm_rep_create(PROTO_VSF_COM_NODE, &init, &rep);

    // init start
    ret = nnm_req_create(PROTO_VSF_COM_NODE, &req);
    assert(!ret);

    // init
    for (i = 0; i < cfg_get_member(stream)->num; i++) {
        uint8_t *obuf = NULL;
        size_t osize  = 0;
        proto_package_fill(ibuf, i, PROTO_VSF_KEY_STREAM, PROTO_ACTION_SET, PROTO_FORMAT_STRUCTE, cfg_get_member(stream)->streams + i, sizeof(proto_vsf_stream_t));
        nnm_req_exchange(req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
        assert(osize == sizeof(proto_header_t));
        memcpy(ibuf, obuf, osize);
        nnm_free(obuf);
    }

    for (i = 0; i < cfg_get_member(frame)->num; i++) {
        uint8_t *obuf = NULL;
        size_t osize  = 0;
        proto_package_fill(ibuf, i, PROTO_VSF_KEY_FRAME, PROTO_ACTION_SET, PROTO_FORMAT_STRUCTE, cfg_get_member(frame)->frames + i, sizeof(proto_vsf_frame_t));
        nnm_req_exchange(req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
        assert(osize == sizeof(proto_header_t));
        memcpy(ibuf, obuf, osize);
        nnm_free(obuf);
    }

    for (i = 0; i < cfg_get_member(osd)->num; i++) {
        uint8_t *obuf = NULL;
        size_t osize  = 0;
        proto_package_fill(ibuf, i, PROTO_VSF_KEY_OSD, PROTO_ACTION_SET, PROTO_FORMAT_STRUCTE, cfg_get_member(osd)->osds + i, sizeof(proto_vsf_osd_t));
        nnm_req_exchange(req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
        assert(osize == sizeof(proto_header_t));
        memcpy(ibuf, obuf, osize);
        nnm_free(obuf);
    }

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