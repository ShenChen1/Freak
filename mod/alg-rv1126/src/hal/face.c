#include "inc/hal/face.h"
#include "NceFace_c.h"
#include "alg_type.h"
#include "log.h"
#include "common.h"
#include "media.h"
#include "proto_app.h"


typedef struct {
    que_t *resultQue;
    pthread_t s_hMdThread;
    pthread_t s_rsltThread;
    float threshold;
    int isLog;
    bool s_bStopSignal;
    app_alg_cb_t cb[APP_ALG_CB_MAX];
    uint8_t *data;
    nce_face_bucket face_bucket;
} app_face_priv_t;

typedef struct {
    app_face_t *objs;
} app_face_mod_t;

static app_face_mod_t s_mod = { 0 };

static void *hs_fd_result_task(void *args)
{
    int ret;
    app_face_priv_t *priv = args;
    void *results         = NULL;

    while (0 == priv->s_bStopSignal) {
        // block mode
        ret = que_get(priv->resultQue, (ptr_t *)&results, 1);
        if (!ret) {
            priv->cb[APP_ALG_CB_RESULT_OUT].func((void *)results, priv->cb[APP_ALG_CB_RESULT_OUT].args);
            free(results);
        }
    }
    return 0;
}
NCE_S32 nce_c_write_img(const char *img_path, img_t *input_img);
static void *hs_fd_task(void *args)
{
    app_face_priv_t *priv = args;
    NceFdResult *resultslist = NULL;
    int num = 0;
    int i = 0;
    int s32Ret = 0;
    long            spend;
    struct timespec start, end;
    img_t rec_img = {0};

    rec_img.image = malloc(640*640*3);
    while (0 == priv->s_bStopSignal) {
        char *data = NULL;

        if (priv->cb[APP_ALG_CB_FRAME_GET].func) {
            s32Ret = priv->cb[APP_ALG_CB_FRAME_GET].func((void *)&data, priv->cb[APP_ALG_CB_FRAME_GET].args);
        }
        if (0 != s32Ret) {
            errorf("get failed!\n");
            continue;
        }

        video_frame_t *frame = NULL;
        frame = (video_frame_t *)data;
        frame->ptr = (void *)((char*)data + sizeof(video_frame_t));
        printf("frame %p data %p %d %d\n",frame,data,frame->u32Width,frame->u32Height);
        img_t detect_img = {0};
        detect_img.image_attr.u32Width = frame->u32Width;
        detect_img.image_attr.u32Height = frame->u32Height;
        detect_img.image_attr.u32channel = 3;
        detect_img.image_attr.order = RGB;
        detect_img.image_attr.format = PACKAGE;
        detect_img.image = frame->ptr;

        rec_img.image_attr = detect_img.image_attr;
        memcpy(rec_img.image,detect_img.image,frame->u32Width*frame->u32Height*3);
        //nce_c_write_img("frame.jpg",&detect_img);

        clock_gettime(0, &start);
        priv->face_bucket.face_det(priv->face_bucket.pPriv,&detect_img,&resultslist,&num);
        clock_gettime(0, &end);
        spend = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
        printf("\n[inference]===== TIME SPEND: %ld ms =====\n", spend);
        for (i = 0; i < num; i++) {
            printf("--------------------id %d,rect(%d,%d,%d,%d)-----------------\n",resultslist[i].id,resultslist[i].rect.centerx,resultslist[i].rect.centery,
            resultslist[i].rect.heigth,resultslist[i].rect.width);
        }

        if (priv->cb[APP_ALG_CB_RESULT_OUT].func) {

        proto_app_alg_result_t *results = malloc(sizeof(proto_app_alg_result_t));
        if (results) {
            memset(results, 0, sizeof(proto_app_alg_result_t));
            results->num = num;
            for (i = 0; i < num; i++) {
                results->objs[i].id     = resultslist[i].id;
                memcpy(results->objs[i].text,"unknow",7);
                results->objs[i].rect.x = resultslist[i].rect.centerx - resultslist[i].rect.width / 2;
                results->objs[i].rect.y = resultslist[i].rect.centery - resultslist[i].rect.heigth / 2;
                results->objs[i].rect.w = resultslist[i].rect.width;
                results->objs[i].rect.h = resultslist[i].rect.heigth;
                // to do keypoint
                results->objs[i].rect.x = results->objs[i].rect.x * 8192 / 640;
                results->objs[i].rect.y = results->objs[i].rect.y * 8192 / 640;
                results->objs[i].rect.w = results->objs[i].rect.w * 8192 / 640;
                results->objs[i].rect.h = results->objs[i].rect.h * 8192 / 640;
                int k = 0;
                for(k = 0; k<5; k++)
                {
                    results->objs[i].keypoints->x = resultslist[i].landmarks[2*k];
                    results->objs[i].keypoints->y = resultslist[i].landmarks[2*k+1];
                }
                clock_gettime(0, &start);
                int id = priv->face_bucket.face_rec(priv->face_bucket.pPriv,&rec_img,resultslist[i].landmarks,0.4);
                printf("\n[recgo]===== TIME SPEND: %ld ms =====\n", spend);
                if(id>=0)
                {
                    NceFaceDbInfo dbinfolist[2];
                    NceFaceDbInfo *p = dbinfolist;
                    priv->face_bucket.facedb_getdata(priv->face_bucket.pPriv,&id,1,&p);
                    memcpy(results->objs[i].text,p[0].name,16);
                }

                
            }
            // no block
            que_put(priv->resultQue, results, 0);
        }
        }

        if (priv->cb[APP_ALG_CB_FRAME_FREE].func) {
            s32Ret = priv->cb[APP_ALG_CB_FRAME_FREE].func(data, priv->cb[APP_ALG_CB_FRAME_FREE].args);
        }
    }
    free(rec_img.image);
    return 0;
}

static int __hs_fd_init(app_face_t *self, char *path)
{
    app_face_t *obj       = self;
    app_face_priv_t *priv = obj->priv;
    infof("fd_init");

    priv->isLog         = 0;
    priv->threshold     = 0.6;
    priv->s_bStopSignal = 0;

    nce_face_bucket_init(&priv->face_bucket,"/var/fd.yaml","/var/fr.yaml");
    que_create(32, &priv->resultQue);

    pthread_create(&priv->s_hMdThread, NULL, hs_fd_task, (void *)priv);
    pthread_create(&priv->s_rsltThread, NULL, hs_fd_result_task, (void *)priv);
    return 0;
}

static int __hs_fd_regcallback(app_face_t *self, int type, app_alg_cb_t *cb)
{
    app_face_t *obj       = self;
    app_face_priv_t *priv = obj->priv;

    if (cb == NULL) {
        priv->cb[APP_ALG_CB_FRAME_GET].args  = NULL;
        priv->cb[APP_ALG_CB_FRAME_GET].func  = NULL;
        priv->cb[APP_ALG_CB_FRAME_FREE].args = NULL;
        priv->cb[APP_ALG_CB_FRAME_FREE].func = NULL;
        priv->cb[APP_ALG_CB_RESULT_OUT].args = NULL;
        priv->cb[APP_ALG_CB_RESULT_OUT].func = NULL;
    } else {
        priv->cb[APP_ALG_CB_FRAME_GET].args  = cb[APP_ALG_CB_FRAME_GET].args;
        priv->cb[APP_ALG_CB_FRAME_GET].func  = cb[APP_ALG_CB_FRAME_GET].func;
        priv->cb[APP_ALG_CB_FRAME_FREE].args = cb[APP_ALG_CB_FRAME_FREE].args;
        priv->cb[APP_ALG_CB_FRAME_FREE].func = cb[APP_ALG_CB_FRAME_FREE].func;
        priv->cb[APP_ALG_CB_RESULT_OUT].args = cb[APP_ALG_CB_RESULT_OUT].args;
        priv->cb[APP_ALG_CB_RESULT_OUT].func = cb[APP_ALG_CB_RESULT_OUT].func;
    }
    return 0;
}

static int __hs_fd_destroy(app_face_t *self)
{
    app_face_t *obj       = self;
    app_face_priv_t *priv = obj->priv;

    priv->s_bStopSignal = 1;
    pthread_join(priv->s_hMdThread, NULL);
    pthread_join(priv->s_rsltThread, NULL);
    nce_face_bucket_deinit(&priv->face_bucket);
    que_delete(priv->resultQue);
    return 0;
}

app_face_t *APP_createFaceAlg(void)
{
    app_face_mod_t *mod   = &s_mod;
    app_face_t *obj       = NULL;
    app_face_priv_t *priv = NULL;

    obj = mod->objs;
    if (obj) {
        return obj;
    }

    priv = malloc(sizeof(app_face_priv_t));
    assert(priv);
    memset(priv, 0, sizeof(app_face_priv_t));
    priv->s_bStopSignal = 1;

    obj = malloc(sizeof(app_face_t));
    assert(obj);
    memset(obj, 0, sizeof(app_face_t));

    obj->priv        = priv;
    obj->init        = __hs_fd_init;
    obj->destroy     = __hs_fd_destroy;
    obj->regcallback = __hs_fd_regcallback;

    mod->objs = obj;
    return obj;
}
