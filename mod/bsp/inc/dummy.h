#ifndef __DUMMY_H__
#define __DUMMY_H__

typedef struct dummy {
    /* Private date */
    void *priv;

    /**
     * @brief  Destory dummy instance
     *
     * @param  self     dummy instance
     * @return 0 on successs, <0 otherwise.
     */
    int (*deinit)(struct dummy *self);

    /**
     * @brief Set dummy value
     *
     * @param  self     dummy instance
     * @param  value    dummy value
     * @return 0 on successs, <0 otherwise.
     */
    int (*set)(struct dummy *self, int value);

    /**
     * @brief Get dummy status
     *
     * @param  self     dummy instance
     * @param  value    dummy value
     * @return 0 on successs, <0 otherwise.
     */
    int (*get)(struct dummy *self, int *value);

} dummy_t;

/**
 * @brief  Create dummy instance
 *
 * @param  id           dummy instance id
 * @return dummy        created instance on success,
 *                      NULL on error
 */
dummy_t *createDummy(int id);

/**
 * @brief  get dummy resource num
 *
 * @return >=0 for the resource num, <0 otherwise.
 */
int getDummyResourceNum();

#endif //__DUMMY_H__