.intel_syntax noprefix
.section .text
.global _start
.code32
_start:
    cli
    mov esp, offset BootStack
.section .bss
.align PAGE_SIZE
.skip PAGE_SIZE
BootStack:
    mov eax, cr4
    or eax, 1<<5
    mov cr4, eax

    // Load P4 page table
    mov eax, offset BootP4
    mov cr3, eax

    // Yaaay! Let's go to long mode
    mov ecx, 0x0C0000080
    rdmsr
    or eax, 1<<8
    wrmsr

    // Enable paging
    mov eax, cr0
    or eax, 1<<31
    mov cr0, eax

    // Load GDT
    lgdt [BootGRTp]

    // Long jump
    jmp 0x8:long_mode_start

.code64
    long_mode_start:
    
    mov eax, 0x0
    mov ss, eax
    mov ds, eax
    mov es, eax

    jmp $
