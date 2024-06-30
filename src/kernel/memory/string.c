#include <memory.h>
#include <stdint.h>
#include <stddef.h>

void *memcpy(void *dst, const void *src, size_t n)
{
    char *dp = dst;
    const char *sp = src;
    while (n--)
        *dp++ = *sp++;
    return dst;
}

void *memset(void *s, int c, size_t n)
{
    unsigned char *p = s;
    while (n--)
        *p++ = (unsigned char)c;
    return s;
}

void *memmove(void *dst, const void *src, size_t n)
{
    if (src == dst)
        return dst;

    const void *src_end = (const void *)((uintptr_t)src + n);
    if (src < dst && dst < src_end)
    {
        char *dp = dst;
        const char *sp = src;
        while (n--)
            dp[n] = sp[n];
        return dst;
    }

    memcpy(dst, src, n);
    return dst;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *p1 = s1, *p2 = s2;
    for (; n--; p1++, p2++)
    {
        if (*p1 != *p2)
            return *p1 - *p2;
    }
    return 0;
}

size_t strlen(const char *s)
{
    size_t length = 0;
    while (*s++)
        length++;
    return length;
}
