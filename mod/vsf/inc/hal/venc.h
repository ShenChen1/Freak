#ifndef __VENC_H__
#define __VENC_H__

typedef struct venc {
    /* Private date */
    void *priv;

    /**
     * @brief  Destroy venc instance
     *
     * @param  self     venc instance
     * @return 0 on successs, <0 otherwise.
     */
    int (*destroy)(struct venc *self);

} venc_t;

/**
 * @brief  Create venc instance
 *
 * @param  id           venc instance id
 * @return venc         created instance on success,
 *                      NULL on error
 */
venc_t *createVenc(int id);

#endif //__VENC_H__