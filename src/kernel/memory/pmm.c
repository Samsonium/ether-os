#include <memory.h>

// Virt addr of the next free page
uint64_t next = 0;

void pmm_free(uint64_t page)
{
    *(uint64_t *)P2V(page) = next;
    next = (uint64_t)P2V(page);
}

uint64_t pmm_alloc()
{
    if (!next)
        return 0;

    uint64_t page = next;
    next = *(uint64_t *)page;
    return (uint64_t)V2P(page);
}

uint64_t pmm_calloc()
{
    uint64_t page = pmm_alloc();
    memset(P2V(page), 0, PAGE_SIZE);
    return page;
}
