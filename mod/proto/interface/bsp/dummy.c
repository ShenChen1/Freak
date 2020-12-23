#include "bsp/dummy.h"
#include "log.h"
#include "nnm.h"
#include "proto.h"

typedef struct {
    int id;
    nnm_t req;
} dummy_priv_t;

static int __bsp_dummy_destroy(bsp_dummy_t *self)
{
    dummy_priv_t *priv = self->priv;

    nnm_req_destory(priv->req);
    free(priv);
    free(self);
    return 0;
}

static int __bsp_dummy_cap(bsp_dummy_t *self, proto_bsp_dummy_cap_t *cap)
{
    dummy_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(cap, 0, sizeof(proto_bsp_dummy_cap_t));
    proto_package_fill(ibuf, priv->id, PROTO_BSP_KEY_DUMMY, PROTO_ACTION_CAP, PROTO_FORMAT_STRUCTE, cap, sizeof(proto_bsp_dummy_cap_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_BSP_KEY_DUMMY);
    assert(!obuf->errcode);

    memcpy(cap, proto_package_data(obuf), sizeof(proto_bsp_dummy_cap_t));
    nnm_free(obuf);

    return 0;
}

static int __bsp_dummy_set(bsp_dummy_t *self, proto_bsp_dummy_cfg_t *cfg)
{
    dummy_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    proto_package_fill(ibuf, priv->id, PROTO_BSP_KEY_DUMMY, PROTO_ACTION_SET, PROTO_FORMAT_STRUCTE, cfg, sizeof(proto_bsp_dummy_cfg_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_BSP_KEY_DUMMY);
    assert(!obuf->errcode);

    assert(proto_package_size(obuf) == sizeof(proto_header_t));
    nnm_free(obuf);

    return 0;
}

static int __bsp_dummy_get(bsp_dummy_t *self, proto_bsp_dummy_cfg_t *cfg)
{
    dummy_priv_t *priv = self->priv;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    memset(cfg, 0, sizeof(proto_bsp_dummy_cfg_t));
    proto_package_fill(ibuf, priv->id, PROTO_BSP_KEY_DUMMY, PROTO_ACTION_GET, PROTO_FORMAT_STRUCTE, cfg, sizeof(proto_bsp_dummy_cfg_t));
    nnm_req_exchange(priv->req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_BSP_KEY_DUMMY);
    assert(!obuf->errcode);

    memcpy(cfg, proto_package_data(obuf), sizeof(proto_bsp_dummy_cfg_t));
    nnm_free(obuf);
    return 0;
}

bsp_dummy_t * __weak bsp_createDummy(int id)
{
    int ret;
    bsp_dummy_t *obj       = NULL;
    dummy_priv_t *priv = NULL;

    priv = malloc(sizeof(dummy_priv_t));
    assert(priv);
    ret = nnm_req_create(PROTO_BSP_COM_NODE, &priv->req);
    assert(!ret);
    priv->id = id;

    obj = malloc(sizeof(bsp_dummy_t));
    assert(obj);
    obj->priv    = priv;
    obj->cap     = __bsp_dummy_cap;
    obj->set     = __bsp_dummy_set;
    obj->get     = __bsp_dummy_get;
    obj->destroy = __bsp_dummy_destroy;

    infof("Create dummy %d : %p", id, obj);
    return obj;
}

int __weak bsp_getDummyNum()
{
    int ret;
    nnm_t req = NULL;
    uint8_t ibuf[PROTO_PACKAGE_MAXSIZE] = {};
    proto_header_t *obuf = NULL;
    size_t osize = 0;

    ret = nnm_req_create(PROTO_BSP_COM_NODE, &req);
    assert(!ret);

    proto_package_fill(ibuf, -1, PROTO_BSP_KEY_DUMMY, PROTO_ACTION_NUM, PROTO_FORMAT_STRUCTE, &ret, sizeof(int));
    nnm_req_exchange(req, ibuf, proto_package_size(ibuf), (void **)&obuf, &osize);
    assert(obuf->key == PROTO_BSP_KEY_DUMMY);
    assert(!obuf->errcode);

    memcpy(&ret, proto_package_data(obuf), sizeof(int));
    nnm_free(obuf);

    nnm_req_destory(req);
    return ret;
}