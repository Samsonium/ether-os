#include <gdt.h>
.intel_syntax noprefix

.section .rodata
.global BootGDT
.global BootGDTp

BootGDT:
    .long 0,0
    .long 0, (GDT_PRESENT | GDT_CODE | GDT_LONG)

BootGRTp:
    .short 2*8-1
    .quad offset BootGDT
