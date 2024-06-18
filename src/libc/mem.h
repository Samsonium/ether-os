#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"

void memory_copy(u8 *source, u8 *dest, int nbytes);
void memory_set(u8 *dest, u8 val, u32 len);

struct boundary_tag
{
    u32 magic;
    u32 size;
    u32 real_size;
    s32 index;

    struct boundary_tag *split_left;
    struct boundary_tag *split_right;

    struct boundary_tag *next;
    struct boundary_tag *prev;
};

extern s32 liballoc_lock();
extern s32 liballoc_unlock();
extern s32 liballoc_free();
extern s32 liballoc_alloc();

void *kern_malloc(size_t);
void *kern_realloc(void *, size_t);
void *kern_calloc(size_t, size_t);
void kern_free(void *);

#endif
