#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "log.h"
#include "inc/dummy.h"

#define DUMMY_RESOURCE_NUM  (3)

typedef struct {
    int value;
} dummy_priv_t;

static dummy_t *s_dummy_obj[DUMMY_RESOURCE_NUM] = {};

static int __dummy_destroy(dummy_t *self)
{
    free(self->priv);
    free(self);
    return 0;
}

static int __dummy_set(dummy_t *self, int value)
{
    dummy_priv_t *priv = self->priv;
    priv->value = value;
    return 0;
}
static int __dummy_get(dummy_t *self, int *value)
{
    dummy_priv_t *priv = self->priv;
    *value = priv->value;
    return 0;
}

dummy_t *createDummy(int id)
{
    dummy_t *obj = NULL;
    dummy_priv_t *priv = NULL;

    if (id >= getDummyResourceNum()) {
        return NULL;
    }

    if (s_dummy_obj[id]) {
        infof("Get dummy %d : %p", id, s_dummy_obj[id]);
        return s_dummy_obj[id];
    }

    priv = malloc(sizeof(dummy_priv_t));
    assert(priv);
    priv->value = 0;

    obj = malloc(sizeof(dummy_t));
    assert(obj);
    obj->priv = priv;
    obj->set = __dummy_set;
    obj->get = __dummy_get;
    obj->destroy = __dummy_destroy;

    s_dummy_obj[id] = obj;
    infof("Create dummy %d : %p", id, s_dummy_obj[id]);
    return s_dummy_obj[id];
}

int getDummyResourceNum()
{
    return DUMMY_RESOURCE_NUM;
}