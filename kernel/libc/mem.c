#include "../include/mem.h"

//
// Code partially taken from https://github.com/blanham/liballoc/blob/master/liballoc.c
//

#define LIBALLOC_MAGIC 0x001C00DE
#define MAXCOMPLETE 5
#define MAXEXP 32
#define MINEXP 8

#define MODE_BEST 0
#define MODE_INSTANT 1

#define MODE MODE_BEST

struct boundary_tag *l_freePages[MAXEXP];
s32 l_completePages[MAXEXP];

static s32 l_initialized = 0;
static s32 l_pageSize = 4096;
static s32 l_pageCount = 16;

void memory_copy(u8 *source, u8 *dest, s32 nbytes)
{
    s32 i;
    for (i = 0; i < nbytes; i++)
    {
        *(dest + i) = *(source + i);
    }
}

void memory_set(u8 *dest, u8 val, u32 len)
{
    u8 *temp = (u8 *)dest;
    for (; len != 0; len--)
        *temp++ = val;
}

static inline s32 getexp(u32 size)
{
    if (size < (1 << MINEXP))
    {
        return -1;
    }

    s32 shift = MINEXP;

    while (shift < MAXEXP)
    {
        if ((1 << shift) > size)
            break;

        shift += 1;
    }

    return shift - 1;
}

static void *liballoc_memset(void *s, s32 c, size_t n)
{
    s32 i;
    for (i = 0; i < n; i++)
        ((s8 *)s)[i] = c;

    return s;
}

void *liballoc_memcpy(void *s1, const void *s2, size_t n)
{
    s8 *cdest;
    s8 *csrc;

    u32 *ldest = (u32 *)s1;
    u32 *lsrc = (u32 *)s2;

    while (n >= sizeof(u32))
    {
        *ldest++ = *lsrc++;
        n -= 1;
    }

    return s1;
}

static inline void insert_tag(struct boundary_tag *tag, s32 index)
{
    s32 realIndex;

    if (index < 0)
    {
        realIndex = getexp(tag->real_size - sizeof(struct boundary_tag));
        if (realIndex < MINEXP)
            realIndex = MINEXP;
    }
    else
        realIndex = index;

    tag->index = realIndex;

    if (l_freePages[realIndex] != NULL)
    {
        l_freePages[realIndex]->prev = tag;
        tag->next = l_freePages[realIndex];
    }

    l_freePages[realIndex] = tag;
}

static inline void remove_tag(struct boundary_tag *tag)
{
    if (l_freePages[tag->index] == tag)
        l_freePages[tag->index] = tag->next;

    if (tag->prev != NULL)
        tag->prev->next = tag->next;
    if (tag->next != NULL)
        tag->next->prev = tag->prev;

    tag->next = NULL;
    tag->prev = NULL;
    tag->index = -1;
}

static inline struct boundary_tag *melt_left(struct boundary_tag *tag)
{
    struct boundary_tag *left = tag->split_left;

    left->real_size += tag->real_size;
    left->split_right = tag->split_right;

    if (tag->split_right != NULL)
        tag->split_right->split_left = left;

    return left;
}

static inline struct boundary_tag *absorb_right(struct boundary_tag *tag)
{
    struct boundary_tag *right = tag->split_right;

    remove_tag(right); // Remove right from free pages.

    tag->real_size += right->real_size;

    tag->split_right = right->split_right;
    if (right->split_right != NULL)
        right->split_right->split_left = tag;

    return tag;
}

static inline struct boundary_tag *split_tag(struct boundary_tag *tag)
{
    u32 remainder = tag->real_size - sizeof(struct boundary_tag) - tag->size;

    struct boundary_tag *new_tag =
        (struct boundary_tag *)((u32)tag + sizeof(struct boundary_tag) + tag->size);

    new_tag->magic = LIBALLOC_MAGIC;
    new_tag->real_size = remainder;

    new_tag->next = NULL;
    new_tag->prev = NULL;

    new_tag->split_left = tag;
    new_tag->split_right = tag->split_right;

    if (new_tag->split_right != NULL)
        new_tag->split_right->split_left = new_tag;
    tag->split_right = new_tag;

    tag->real_size -= new_tag->real_size;

    insert_tag(new_tag, -1);

    return new_tag;
}

static struct boundary_tag *allocate_new_tag(u32 size)
{
    u32 pages;
    u32 usage;
    struct boundary_tag *tag;

    usage = size + sizeof(struct boundary_tag);

    pages = usage / l_pageSize;
    if ((usage % l_pageSize) != 0)
        pages += 1;

    if (pages < l_pageCount)
        pages = l_pageCount;

    tag = (struct boundary_tag *)liballoc_alloc(pages);

    if (tag == NULL)
        return NULL;

    tag->magic = LIBALLOC_MAGIC;
    tag->size = size;
    tag->real_size = pages * l_pageSize;
    tag->index = -1;

    tag->next = NULL;
    tag->prev = NULL;
    tag->split_left = NULL;
    tag->split_right = NULL;

    return tag;
}

void *kern_malloc(size_t size)
{
    s32 index;
    void *ptr;
    struct boundary_tag *tag = NULL;

    liballoc_lock();

    if (l_initialized == 0)
    {
        for (index = 0; index < MAXEXP; index++)
        {
            l_freePages[index] = NULL;
            l_completePages[index] = 0;
        }
        l_initialized = 1;
    }

    index = getexp(size) + MODE;
    if (index < MINEXP)
        index = MINEXP;

    tag = l_freePages[index];
    while (tag != NULL)
    {
        if ((tag->real_size - sizeof(struct boundary_tag)) >= (size + sizeof(struct boundary_tag)))
            break;

        tag = tag->next;
    }

    if (tag == NULL)
    {
        if ((tag = allocate_new_tag(size)) == NULL)
        {
            liballoc_unlock();
            return NULL;
        }

        index = getexp(tag->real_size - sizeof(struct boundary_tag));
    }
    else
    {
        remove_tag(tag);
        if ((tag->split_left == NULL) && (tag->split_right == NULL))
            l_completePages[index] -= 1;
    }

    tag->size = size;
    u32 remainder = tag->real_size - size - sizeof(struct boundary_tag) * 2;

    if ((s32)(remainder) > 0)
    {
        s32 childIndex = getexp(remainder);

        if (childIndex >= 0)
        {
            struct boundary_tag *new_tag = split_tag(tag);
            new_tag = new_tag;
        }
    }

    ptr = (void *)((u32)tag + sizeof(struct boundary_tag));

    liballoc_unlock();
    return ptr;
}

void kern_free(void *ptr)
{
    s32 index;
    struct boundary_tag *tag;

    liballoc_lock();

    tag = (struct boundary_tag *)((u32)ptr - sizeof(struct boundary_tag));
    if (tag->magic != LIBALLOC_MAGIC)
    {
        liballoc_unlock();
        return;
    }

    while ((tag->split_left != NULL) && (tag->split_left->index >= 0))
    {
        tag = melt_left(tag);
        remove_tag(tag);
    }

    while ((tag->split_right != NULL) && (tag->split_right->index >= 0))
    {
        tag = absorb_right(tag);
    }

    index = getexp(tag->real_size - sizeof(struct boundary_tag));
    if (index < MINEXP)
        index = MINEXP;

    if ((tag->split_left == NULL) && (tag->split_right == NULL))
    {
        if (l_completePages[index] == MAXCOMPLETE)
        {
            u32 pages = tag->real_size / l_pageSize;

            if ((tag->real_size % l_pageSize) != 0)
                pages += 1;

            if (pages < l_pageCount)
                pages = l_pageCount;

            liballoc_free(tag, pages);
            liballoc_unlock();
            return;
        }

        l_completePages[index] += 1;
    }

    insert_tag(tag, index);
    liballoc_unlock();
}

void *kern_calloc(size_t nobj, size_t size)
{
    s32 real_size;
    void *p;

    real_size = nobj * size;
    p = kern_malloc(real_size);
    liballoc_memset(p, 0, real_size);
    return p;
}

void *kern_realloc(void *p, size_t size)
{
    void *ptr;
    struct boundary_tag *tag;
    s32 real_size;

    if (size == 0)
    {
        kern_free(p);
    }

    if (p == NULL)
        return kern_malloc(size);

    if (liballoc_lock != NULL)
        liballoc_lock();

    tag = (struct boundary_tag *)((u32)p - sizeof(struct boundary_tag));
    real_size = tag->size;

    if (liballoc_unlock != NULL)
        liballoc_unlock();

    if (real_size > size)
        real_size = size;

    ptr = kern_malloc(size);
    liballoc_memcpy(ptr, p, real_size);
    kern_free(p);

    return ptr;
}
