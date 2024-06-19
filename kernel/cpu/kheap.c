#include "../include/kheap.h"

u32 placement_address = (u32)0x10000;

u32 kmalloc(size_t size)
{
    u32 mem = placement_address;
    placement_address += size;
    return mem;
}

u32 kmalloc_page()
{
    if (placement_address & 0xFFFFF000)
    {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    return kmalloc(0x1000);
}
