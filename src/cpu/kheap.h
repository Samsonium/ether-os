#ifndef KHEAP_H
#define KHEAP_H

#include "types.h"
#include "../libc/mem.h"

u32 kmalloc(size_t size);
u32 kmalloc_page();

#endif
