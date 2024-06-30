#pragma once

#include <stdint.h>

static __inline void _outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %1, %0" ::"dN"(port), "a"(value));
}

static __inline uint8_t _inb(uint16_t port)
{
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "dN"(port));
    return result;
}

#define outb _outb
#define inb _inb
