#include "gdt.h"

gdt_entry_t gdt[6];
gdt_desc_t info;

void gdt_init()
{
    gdt_entry_add(1, true, 0);
    gdt_entry_add(2, false, 0);
    gdt_entry_add(3, true, 3);
    gdt_entry_add(4, false, 3);

    info.size = sizeof(gdt_entry_t) * 6 - 1;
    info.offset = (uint32_t)&gdt;

    __asm__("lgdt %0" ::"m"(info));
}

void gdt_entry_add(int i, bool exec, uint8_t privelege)
{
    gdt[i].l0_15 = 0xFFFF;
    gdt[i].b0_15 = 0x0000;
    gdt[i].b16_23 = 0x00;
    gdt[i].accessed = 0;
    gdt[i].rw = 1;
    gdt[i].dir = 0;
    gdt[i].exec = exec;
    gdt[i].r1 = 1;
    gdt[i].privilege_level = privelege;
    gdt[i].present = 1;
    gdt[i].l16_19 = 0xf;
    gdt[i].r2 = 0;
    gdt[i].size = 1;
    gdt[i].granularity = 1;
    gdt[i].b24_31 = 0x00;
}
