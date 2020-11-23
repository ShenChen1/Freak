#ifndef __PHYMEM_H__
#define __PHYMEM_H__

void *phymap(off_t phyaddr, size_t size);
int phyunmap(void *virtaddr, size_t size);

#endif /* __PHYMEM_H__ */