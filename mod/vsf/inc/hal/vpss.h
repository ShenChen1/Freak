#ifndef __VPSS_H__
#define __VPSS_H__

typedef struct vpss {
    /* Private date */
    void *priv;

    /**
     * @brief  Destory vpss instance
     *
     * @param  self     vpss instance
     * @return 0 on successs, <0 otherwise.
     */
    int (*destory)(struct vpss *self);

} vpss_t;

/**
 * @brief  Create vpss instance
 *
 * @param  id           vpss instance id
 * @return vpss         created instance on success,
 *                      NULL on error
 */
vpss_t *createVpss(int id);

#endif //__VPSS_H__