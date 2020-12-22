#ifndef __BSP_DUMMY_H__
#define __BSP_DUMMY_H__

#include "proto_bsp.h"

typedef struct dummy {
    /* Private date */
    void *priv;

    /**
     * @brief  Destroy dummy instance
     *
     * @param  self     dummy instance
     * @return 0 on successs, <0 otherwise.
     */
    int (*destroy)(struct dummy *self);

    /**
     * @brief Get dummy capability
     *
     * @param  self     dummy instance
     * @param  cap      dummy capability
     * @return 0 on successs, <0 otherwise.
     */
    int (*cap)(struct dummy *self, proto_bsp_dummy_cap_t *cap);

    /**
     * @brief Set dummy configuration
     *
     * @param  self     dummy instance
     * @param  cfg      dummy configuration
     * @return 0 on successs, <0 otherwise.
     */
    int (*set)(struct dummy *self, proto_bsp_dummy_cfg_t *cfg);

    /**
     * @brief Get dummy configuration
     *
     * @param  self     dummy instance
     * @param  cfg      dummy configuration
     * @return 0 on successs, <0 otherwise.
     */
    int (*get)(struct dummy *self, proto_bsp_dummy_cfg_t *cfg);

} dummy_t;

/**
 * @brief  Create dummy instance
 *
 * @param  id           dummy instance id
 * @return dummy        created instance on success,
 *                      NULL on error
 */
dummy_t *createDummy(int id);
dummy_t *createDummy_r(int id);

/**
 * @brief  get the number of dummy
 *
 * @return >=0 for the number, <0 otherwise.
 */
int getDummyNum();
int getDummyNum_r();

#endif //__BSP_DUMMY_H__