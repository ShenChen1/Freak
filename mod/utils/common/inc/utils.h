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

#endif /* __UTILS_H__ */