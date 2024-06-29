#include <memory.h>
.intel_syntax noprefix

.section .data
.align PAGE_SIZE
.global BootP4

BootP4:
    .quad offset BootP3 + (PAGE_PRESENT | PAGE_WRITE)
    .rept ENTRIES_PER_PT - 1
        .quad 0
    .endr

BootP3:
    .quad offset BootP2 + (PAGE_PRESENT | PAGE_WRITE)
    .rept ENTRIES_PER_PT - 1
        .quad 0
    .endr

BootP2:
    .quad offset BootP1 + (PAGE_PRESENT | PAGE_WRITE)
    .rept ENTRIES_PER_PT - 1
        .quad 0
    .endr

BootP1:
    .set i, 0
    .rept ENTRIES_PER_PT - 1
        .quad (i << 12) + (PAGE_PRESENT | PAGE_WRITE)
        .set i, (i + 1)
    .endr