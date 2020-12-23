#ifndef __MM_H__
#define __MM_H__

#include <stdio.h>

void *physmap(off_t phyaddr, size_t size);
int physunmap(void *virtaddr, size_t size);

off_t virt2phys(void *virtaddr);

#endif /* __MM_H__ */