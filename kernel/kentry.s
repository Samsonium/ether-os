global _start
[bits 32]

_start:
    [extern kmain]	; Define calling point. Name defined in kernel.c
    call kmain		; Calls the C function
    jmp $
