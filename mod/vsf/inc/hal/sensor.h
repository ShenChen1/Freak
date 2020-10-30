#ifndef __VSF_SENSOR_H__
#define __VSF_SENSOR_H__

typedef struct sensor {
    void *priv;
    int (*destroy)(struct sensor *self);
} vsf_sensor_t;

vsf_sensor_t *VSF_createSensor(int id);
int VSF_getSensorNum(void);

#endif //__VSF_SENSOR_H__