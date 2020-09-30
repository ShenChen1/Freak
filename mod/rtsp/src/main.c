#include "common.h"
#include "inc/msgbox.h"
#include "inc/cfg.h"
#include "inc/rtsp.h"
#include "inc/rtsp-ctrl.h"
#include "nnm.h"
#include "log.h"
#include "ufifo.h"

static void *test_media(void *arg)
{
    FILE *fp = NULL;
    char path[] = "/mnt/hgfs/WinShare/1.h264_frame";
    static uint8_t data[8 * 1024];
    static size_t len = 0;
    fp = fopen(path, "r");
    len = fread(data, 1, sizeof(data), fp);
    debugf("begin: 0x%02x 0x%02x 0x%02x", data[0], data[1], data[2]);
    debugf("begin: 0x%02x 0x%02x 0x%02x", data[3], data[4], data[5]);
    debugf("end: 0x%02x 0x%02x 0x%02x", data[len - 6], data[len - 5], data[len - 4]);
    debugf("end: 0x%02x 0x%02x 0x%02x", data[len - 3], data[len - 2], data[len - 1]);
    fclose(fp);

    ufifo_t *fifo = NULL;
    ufifo_init_t init = {
        .lock = UFIFO_LOCK_NONE,
        .opt = UFIFO_OPT_ALLOC,
        .alloc = {512 * 1024},
        .hook = {NULL, NULL},
    };
    char name[64];
    snprintf(name, sizeof(name), PROTO_VENC_MEDIA_FIFO, "test");
    ufifo_open(name, &init, &fifo);

    while (1) {
        ufifo_put_block(fifo, data, len);
        usleep(40 * 1000);
    }

    ufifo_close(fifo);
    return NULL;
}

static void *test_req(void *arg)
{
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    uint8_t *obuf = NULL;
    size_t osize = 0;
    nnm_t req = arg;
    proto_rtsp_url_t rtsp_url = {"rtsp://admin@127.0.0.1:1234/test"};

    while (1) {
        proto_package_fill(ibuf, 0, PROTP_RTSP_KEY_OPEN, PROTO_ACTION_SET,
            PROTO_FORMAT_STRUCTE, &rtsp_url, sizeof(proto_rtsp_url_t));
        nnm_req_exchange(req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
        assert(osize == sizeof(proto_header_t));
        memcpy(ibuf, obuf, osize);
        nnm_free(obuf);

        sleep(2);

        proto_package_fill(ibuf, 0, PROTP_RTSP_KEY_CLOSE, PROTO_ACTION_SET,
            PROTO_FORMAT_STRUCTE, &rtsp_url, sizeof(proto_rtsp_url_t));
        nnm_req_exchange(req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
        assert(osize == sizeof(proto_header_t));
        memcpy(ibuf, obuf, osize);
        nnm_free(obuf);

        sleep(2);
    }

    return NULL;
}

static int __rep_recv(void *in, size_t isize, void **out, size_t *osize, void *arg)
{
    *out = arg;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main()
{
    void *server = NULL;
    nnm_t rep = NULL;
    nnm_t req = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    log_setlevel(LOG_LV_DEBUG);
    cfg_load(PROTO_RTSP_CFG_PATH);
    msgbox_init();
    server = rtsp_server_init("0.0.0.0", 1234);

    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = {__rep_recv, obuf};
    nnm_rep_create(PROTO_RTSP_COM_NODE, &init, &rep);
    nnm_req_create(PROTO_RTSP_COM_NODE, &req);

    pthread_t thread_media;
    pthread_create(&thread_media, NULL, test_media, NULL);
    pthread_t thread_req;
    pthread_create(&thread_req, NULL, test_req, req);

    rtsp_ctrl_init();
    while (1) {
        infof("keep alive");
        rtsp_ctrl_thread(NULL);
    }
    rtsp_ctrl_uninit();

    nnm_req_destory(req);
    nnm_rep_destory(rep);
    rtsp_server_uninit(server);
    msgbox_deinit();
    log_deinit();

    return 0;
}