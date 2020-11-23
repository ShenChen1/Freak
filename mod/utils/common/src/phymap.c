#include "common.h"

void *phymap(off_t phyaddr, size_t size)
{
    off_t offset  = phyaddr;
    size_t length = size;

    // Truncate offset to a multiple of the page size, or mmap will fail.
    size_t pagesize   = sysconf(_SC_PAGE_SIZE);
    off_t page_base   = (offset / pagesize) * pagesize;
    off_t page_offset = offset - page_base;

    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("Can't open memory");
        return NULL;
    }

    unsigned char *mem = mmap(NULL, page_offset + length, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, page_base);
    if (mem == MAP_FAILED) {
        perror("Can't map memory");
        goto end;
    } else {
        mem = &mem[page_offset];
    }

end:
    close(fd);
    return mem;
}

int phyunmap(void *virtaddr, size_t size)
{
    off_t offset  = (off_t)virtaddr;
    size_t length = size;

    // Truncate offset to a multiple of the page size, or munmap will fail.
    size_t pagesize   = sysconf(_SC_PAGE_SIZE);
    off_t page_base   = (offset / pagesize) * pagesize;
    off_t page_offset = offset - page_base;

    return munmap((void *)page_base, page_offset + length);
}