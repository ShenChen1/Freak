#include "bsp/dummy.h"
#include "common.h"
#include "inc/cfg.h"
#include "log.h"

typedef struct {
    int id;
    int value;
    proto_bsp_dummy_cap_t *cap;
} dummy_priv_t;

static bsp_dummy_t *s_dummy_obj[BSP_ITEM_MAX] = {};

static int __bsp_dummy_destroy(bsp_dummy_t *self)
{
    free(self->priv);
    free(self);
    return 0;
}

static int __bsp_dummy_cap(bsp_dummy_t *self, proto_bsp_dummy_cap_t *cap)
{
    dummy_priv_t *priv = self->priv;

    memcpy(cap, priv->cap, sizeof(proto_bsp_dummy_cap_t));
    return 0;
}

static int __bsp_dummy_set(bsp_dummy_t *self, proto_bsp_dummy_cfg_t *cfg)
{
    dummy_priv_t *priv = self->priv;

    priv->value = cfg->value;
    return 0;
}

static int __bsp_dummy_get(bsp_dummy_t *self, proto_bsp_dummy_cfg_t *cfg)
{
    dummy_priv_t *priv = self->priv;

    cfg->value = priv->value;
    return 0;
}

bsp_dummy_t *bsp_createDummy(int id)
{
    bsp_dummy_t *obj       = NULL;
    dummy_priv_t *priv = NULL;

    if (id >= cfg_get_member(dummy)->num) {
        return NULL;
    }

    if (s_dummy_obj[id]) {
        infof("Get dummy %d : %p", id, s_dummy_obj[id]);
        return s_dummy_obj[id];
    }

    priv = malloc(sizeof(dummy_priv_t));
    assert(priv);
    priv->value = 0;
    priv->id = id;
    priv->cap = &cfg_get_member(dummy)->caps[priv->id];

    obj = malloc(sizeof(bsp_dummy_t));
    assert(obj);
    obj->priv    = priv;
    obj->cap     = __bsp_dummy_cap;
    obj->set     = __bsp_dummy_set;
    obj->get     = __bsp_dummy_get;
    obj->destroy = __bsp_dummy_destroy;

    s_dummy_obj[id] = obj;
    infof("Create dummy %d : %p", id, s_dummy_obj[id]);
    return s_dummy_obj[id];
}

int bsp_getDummyNum()
{
    return cfg_get_member(dummy)->num;
}