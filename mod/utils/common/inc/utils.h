#ifndef __UTILS_H__
#define __UTILS_H__

#define __weak __attribute__((weak))

#define ARRAY_SIZE(ary) (sizeof((ary))/sizeof(*(ary)))

#define container_of(ptr, type, member) \
    (type *)((char *)(ptr) - (char *) &((type *)0)->member)

#define min(x, y) ({                                \
    typeof(x) _min1 = (x);                          \
    typeof(y) _min2 = (y);                          \
    (void) ((void *)&_min1 == (void *)&_min2);      \
    _min1 < _min2 ? _min1 : _min2; })

#define max(x, y) ({                                \
    typeof(x) _max1 = (x);                          \
    typeof(y) _max2 = (y);                          \
    (void) ((void *)&_max1 == (void *)&_max2);      \
    _max1 > _max2 ? _max1 : _max2; })

#define range(v, min, max) ({                       \
    typeof(v) _v = (v);                             \
    typeof(min) _min = (min);                       \
    typeof(max) _max = (max);                       \
    (void) (_v = _v > _max ? _max : _v);            \
    _v < _min ? _min : _v; })

#endif /* __UTILS_H__ */