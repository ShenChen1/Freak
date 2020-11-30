#include "common.h"

/*
 * ``/proc/pid/pagemap``.  This file lets a userspace process find out which
   physical frame each virtual page is mapped to.  It contains one 64-bit
   value for each virtual page, containing the following data (from
   ``fs/proc/task_mmu.c``, above pagemap_read):

    * Bits 0-54  page frame number (PFN) if present
    * Bits 0-4   swap type if swapped
    * Bits 5-54  swap offset if swapped
    * Bit  55    pte is soft-dirty (see
      :ref:`Documentation/admin-guide/mm/soft-dirty.rst <soft_dirty>`)
    * Bit  56    page exclusively mapped (since 4.2)
    * Bits 57-60 zero
    * Bit  61    page is file-page or shared-anon (since 3.5)
    * Bit  62    page swapped
    * Bit  63    page present
*/

#define PAGEMAP_ENTRY (8)
#define GET_BIT(X, Y) (X & (1ULL << Y)) >> Y
#define GET_PFN(X) (X & ((1ULL << 55) - 1))

typedef struct {
    uint64_t pfn : 55;
    unsigned int soft_dirty : 1;
    unsigned int file_page : 1;
    unsigned int swapped : 1;
    unsigned int present : 1;
} pagemap_entry_t;

off_t virt2phys(void *virtaddr)
{
    off_t vaddr     = (off_t)virtaddr;
    size_t pagesize = sysconf(_SC_PAGE_SIZE);

    int pagemap_fd = open("/proc/self/pagemap", O_RDONLY);
    if (pagemap_fd < 0) {
        return 0;
    }

    uint8_t data[PAGEMAP_ENTRY];
    size_t offset = vaddr / pagesize * sizeof(data);
    if (pread(pagemap_fd, data, sizeof(data), offset) != sizeof(data)) {
        close(pagemap_fd);
        return 0;
    }

    int i;
    uint64_t info = 0;
    for (i = 0; i < PAGEMAP_ENTRY; i++) {
        info = (info << 8) + data[i];
    }

    pagemap_entry_t entry;
    entry.pfn        = GET_PFN(info);
    entry.soft_dirty = GET_BIT(info, 55);
    entry.file_page  = GET_BIT(info, 61);
    entry.swapped    = GET_BIT(info, 62);
    entry.present    = GET_BIT(info, 63);
    close(pagemap_fd);

    if (entry.present) {
        if (entry.swapped) {
            return 0;
        } else {
            return entry.pfn * pagesize + vaddr % sysconf(_SC_PAGE_SIZE);
        }
    }

    return 0;
}