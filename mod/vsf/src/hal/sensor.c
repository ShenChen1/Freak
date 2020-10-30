#include "common.h"
#include "inc/hal/sensor.h"

vsf_sensor_t * __weak VSF_createSensor(int id)
{
    return NULL;
}

int __weak VSF_getSensorNum(void)
{
    return -1;
}