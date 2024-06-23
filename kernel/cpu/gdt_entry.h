#ifndef GDT_ENTRY_H
#define GDT_ENTRY_H

#include <stdint.h>

typedef struct gdt_entry
{
    uint16_t l0_15;
    uint16_t b0_15;
    uint8_t b16_23;

    uint8_t accessed : 1;
    uint8_t rw : 1;
    uint8_t dir : 1;
    uint8_t exec : 1;
    uint8_t r1 : 1;

    uint8_t privilege_level : 2;
    uint8_t present : 1;

    uint8_t l16_19 : 4;
    uint8_t r2 : 2;

    uint8_t size : 1;
    uint8_t granularity : 1;

    uint8_t b24_31;

} __attribute__((packed)) gdt_entry_t;

#endif
