#include "bsp/dummy.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    int id;
    nnm_t req;
    int proto;
} bsp_dummy_priv_t;

static int __bsp_dummy_destroy(bsp_dummy_t *self)
{
    bsp_dummy_priv_t *priv = self->priv;

    nnm_req_destory(priv->req);
    free(priv);
    free(self);
    return 0;
}

static int __bsp_dummy_cap(bsp_dummy_t *self, proto_bsp_dummy_cap_t *cap)
{
    bsp_dummy_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_bsp_dummy_cap_t,
                          cap,
                          sizeof(proto_bsp_dummy_cap_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, priv->id, PROTO_BSP_KEY_DUMMY, PROTO_ACTION_CAP, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_BSP_KEY_DUMMY);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_bsp_dummy_cap_t,
                           obuf->data,
                           obuf->size,
                           cap,
                           sizeof(proto_bsp_dummy_cap_t));
    nnm_free(obuf);
    return 0;
}

static int __bsp_dummy_set(bsp_dummy_t *self, proto_bsp_dummy_cfg_t *cfg)
{
    bsp_dummy_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_bsp_dummy_cfg_t,
                          cfg,
                          sizeof(proto_bsp_dummy_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, priv->id, PROTO_BSP_KEY_DUMMY, PROTO_ACTION_SET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_BSP_KEY_DUMMY);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_bsp_dummy_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_bsp_dummy_cfg_t));
    nnm_free(obuf);
    return 0;
}

static int __bsp_dummy_get(bsp_dummy_t *self, proto_bsp_dummy_cfg_t *cfg)
{
    bsp_dummy_priv_t *priv = self->priv;
    proto_header_t *obuf   = NULL;
    size_t osize           = 0;
    size_t isize           = 0;

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(priv->proto,
                          jsonb_opt_proto_bsp_dummy_cfg_t,
                          cfg,
                          sizeof(proto_bsp_dummy_cfg_t),
                          proto_package_data(ibuf),
                          &isize);

    proto_package_fill_header(ibuf, priv->id, PROTO_BSP_KEY_DUMMY, PROTO_ACTION_GET, priv->proto, isize);
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_BSP_KEY_DUMMY);
    assert(!obuf->errcode);

    proto_client_data_post(priv->proto,
                           jsonb_opt_proto_bsp_dummy_cfg_t,
                           obuf->data,
                           obuf->size,
                           cfg,
                           sizeof(proto_bsp_dummy_cfg_t));
    nnm_free(obuf);
    return 0;
}

static bsp_dummy_t *__bsp_createDummy(int id, int proto)
{
    int ret;
    bsp_dummy_t *obj       = NULL;
    bsp_dummy_priv_t *priv = NULL;

    priv = malloc(sizeof(bsp_dummy_priv_t));
    assert(priv);
    ret = nnm_req_create(PROTO_BSP_COM_NODE, &priv->req);
    assert(!ret);
    priv->id    = id;
    priv->proto = proto;

    obj = malloc(sizeof(bsp_dummy_t));
    assert(obj);
    obj->priv    = priv;
    obj->cap     = __bsp_dummy_cap;
    obj->set     = __bsp_dummy_set;
    obj->get     = __bsp_dummy_get;
    obj->destroy = __bsp_dummy_destroy;

    return obj;
}

static int __bsp_getDummyNum(int proto)
{
    int ret;
    nnm_t req            = NULL;
    proto_header_t *obuf = NULL;
    size_t osize         = 0;
    size_t isize         = 0;
    proto_num_t num      = {};

    ret = nnm_req_create(PROTO_BSP_COM_NODE, &req);
    assert(!ret);

    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_client_data_pre(proto, jsonb_opt_proto_num_t, &num, sizeof(proto_num_t), proto_package_data(ibuf), &isize);

    proto_package_fill_header(ibuf, -1, PROTO_BSP_KEY_DUMMY, PROTO_ACTION_NUM, proto, isize);
    nnm_req_exchange(req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_BSP_KEY_DUMMY);
    assert(!obuf->errcode);

    proto_client_data_post(proto, jsonb_opt_proto_num_t, obuf->data, obuf->size, &num, sizeof(proto_num_t));
    nnm_free(obuf);

    nnm_req_destory(req);
    return num.num;
}

bsp_dummy_t *__weak bsp_createDummy(int id)
{
    return __bsp_createDummy(id, PROTO_FORMAT_STRUCTE);
}

bsp_dummy_t *bsp_createDummy_r(int id)
{
    return __bsp_createDummy(id, PROTO_FORMAT_JSON);
}

int __weak bsp_getDummyNum()
{
    return __bsp_getDummyNum(PROTO_FORMAT_STRUCTE);
}

int bsp_getDummyNum_r()
{
    return __bsp_getDummyNum(PROTO_FORMAT_JSON);
}