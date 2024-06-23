#ifndef GDT_DESC_H
#define GDT_DESC_H

#include <stdint.h>

typedef struct gdt_desc
{
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) gdt_desc_t;

#endif
