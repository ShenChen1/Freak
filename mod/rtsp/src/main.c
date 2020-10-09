#include "common.h"
#include "inc/cfg.h"
#include "inc/msgbox.h"
#include "inc/rtsp-ctrl.h"
#include "inc/rtsp.h"
#include "log.h"
#include "nnm.h"
#include "ufifo.h"

#define MEDIA (1)
#if MEDIA
#define H264_NAL(v) (v & 0x1F)
enum { NAL_IDR = 5, NAL_SEI = 6, NAL_SPS = 7, NAL_PPS = 8 };

static uint8_t* search_start_code(uint8_t* ptr, uint8_t* end)
{
    uint8_t* p;
    for (p = ptr; p + 3 < end; p++) {
        if (0x00 == p[0] && 0x00 == p[1] &&
            (0x01 == p[2] || (0x00 == p[2] && 0x01 == p[3])))
            return p;
    }
    return end;
}

static int h264_nal_type(unsigned char* ptr)
{
    int i = 2;
    assert(0x00 == ptr[0] && 0x00 == ptr[1]);
    if (0x00 == ptr[2])
        ++i;
    assert(0x01 == ptr[i]);
    return H264_NAL(ptr[i + 1]);
}

static int h264_nal_new_access(unsigned char* ptr, uint8_t* end)
{
    int i = 2;
    if (end - ptr < 4)
        return 1;
    assert(0x00 == ptr[0] && 0x00 == ptr[1]);
    if (0x00 == ptr[2])
        ++i;
    assert(0x01 == ptr[i]);
    int nal_unit_type = H264_NAL(ptr[i + 1]);
    if (nal_unit_type < 1 || nal_unit_type > 5)
        return 1;

    if (ptr + i + 2 > end)
        return 1;

    // Live555 H264or5VideoStreamParser::parse
    // The high-order bit of the byte after the "nal_unit_header" tells us
    // whether it's the start of a new 'access unit' (and thus the current NAL
    // unit ends an 'access unit'):
    return (ptr[i + 2] & 0x80) != 0 ? 1 : 0;
}

static void* test_media(void* arg)
{
    FILE* fp = NULL;
    char* path = arg;
    uint8_t* data;
    size_t capacity = 0;
    fp = fopen(path, "r");
    fseek(fp, 0, SEEK_END);
    capacity = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    data = malloc(capacity);
    assert(fread(data, 1, capacity, fp) == capacity);
    fclose(fp);

    ufifo_t* fifo = NULL;
    ufifo_init_t init = {
        .lock = UFIFO_LOCK_NONE,
        .opt = UFIFO_OPT_ALLOC,
        .alloc = {64 * 1024},
        .hook = {NULL, NULL},
    };
    char name[64];
    snprintf(name, sizeof(name), PROTO_VENC_MEDIA_FIFO, "test");
    ufifo_open(name, &init, &fifo);

    while (1) {
        uint8_t* end = data + capacity;
        uint8_t* nalu = search_start_code(data, end);
        uint8_t* p = nalu;

        while (p < end) {
            unsigned char* pn = search_start_code(p + 4, end);
            size_t bytes = pn - nalu;
            int nal_unit_type = h264_nal_type(p);
            assert(0 != nal_unit_type);

            if (nal_unit_type <= 5 && h264_nal_new_access(pn, end)) {
                ufifo_put_block(fifo, nalu, bytes);
                usleep(40 * 1000);
                nalu = pn;
            }
            p = pn;
        }
    }

    ufifo_destroy(fifo);
    return NULL;
}
#endif

#if DEBUG
static void* test_req(void* arg)
{
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    uint8_t* obuf = NULL;
    size_t osize = 0;
    nnm_t req = arg;
    proto_rtsp_url_t rtsp_url = {"rtsp://admin@127.0.0.1:1234/test"};

    // wait for media ready
    sleep(5);

    while (1) {
        proto_package_fill(ibuf, 0, PROTP_RTSP_KEY_OPEN, PROTO_ACTION_SET,
                           PROTO_FORMAT_STRUCTE, &rtsp_url,
                           sizeof(proto_rtsp_url_t));
        nnm_req_exchange(req, ibuf, proto_package_size(ibuf), (void**)&obuf,
                         &osize);
        assert(osize == sizeof(proto_header_t));
        memcpy(ibuf, obuf, osize);
        nnm_free(obuf);

        sleep(1);

        proto_package_fill(ibuf, 0, PROTP_RTSP_KEY_CLOSE, PROTO_ACTION_SET,
                           PROTO_FORMAT_STRUCTE, &rtsp_url,
                           sizeof(proto_rtsp_url_t));
        nnm_req_exchange(req, ibuf, proto_package_size(ibuf), (void**)&obuf,
                         &osize);
        assert(osize == sizeof(proto_header_t));
        memcpy(ibuf, obuf, osize);
        nnm_free(obuf);

        sleep(1);
    }

    return NULL;
}
#endif

static int __rep_recv(void* in,
                      size_t isize,
                      void** out,
                      size_t* osize,
                      void* arg)
{
    *out = arg;
    return msgbox_do_handler(in, isize, *out, osize);
}

int main(int argc, char* argv[])
{
    void* server = NULL;
    nnm_t rep = NULL;
    nnm_t req = NULL;

    log_init(PROTO_LOG_COM_NODE, false);
    log_setlevel(LOG_LV_DEBUG);
    cfg_load(PROTO_RTSP_CFG_PATH);
    msgbox_init();
    server = rtsp_server_init("0.0.0.0", *cfg_get_member(port));

    static uint8_t obuf[PROTO_PACKAGE_MAXSIZE];
    nnm_rep_init_t init = {__rep_recv, obuf};
    nnm_rep_create(PROTO_RTSP_COM_NODE, &init, &rep);
    nnm_req_create(PROTO_RTSP_COM_NODE, &req);

#if MEDIA
    pthread_t thread_media;
    pthread_create(&thread_media, NULL, test_media, argv[1]);
#endif
#if DEBUG
    pthread_t thread_req;
    pthread_create(&thread_req, NULL, test_req, req);
#endif

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