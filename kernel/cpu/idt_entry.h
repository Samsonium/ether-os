#ifndef IDT_ENTRY_H
#define IDT_ENTRY_H

#include <stdint.h>

typedef enum idt_egt
{
    Task16 = 0x5,
    Int16 = 0x6,
    Trap16 = 0x7,

    Int32 = 0xE,
    Trap32 = 0xF
} idt_edt_t;

typedef struct idt_entry
{
    uint16_t o0_15;
    uint16_t sel;
    uint8_t r;
    uint8_t type : 4;
    uint8_t storage_seg : 1;
    uint8_t privelege : 2;
    uint8_t present : 1;
    uint16_t o16_31;
} __attribute__((packed)) idt_entry_t;

#endif
