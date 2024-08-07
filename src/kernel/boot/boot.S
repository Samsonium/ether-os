#include <memory.h>
.intel_syntax noprefix

.section .bss
.align PAGE_SIZE
.skip PAGE_SIZE
BootStack:

.section .text

.code32
.global _start
_start:
    cli

    mov edi, eax
    mov esi, ebx

    // Set up a known stack
    mov esp, offset V2P(BootStack)

    // Enable "Physical Address Extension"
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    // Load a P4 page table
    mov eax, offset V2P(BootP4)
    mov cr3, eax

    // Enable long mode
    mov ecx, 0x0C0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    // Enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    // Load a new GDT
    lgdt [V2P(GDTp)]

    // Update code selector by a long jump
    jmp 0x8:V2P(long_mode_start)

.code64
    long_mode_start:
    
    // Clear out all other selectors
    mov eax, 0x0
    mov ss, eax
    mov ds, eax
    mov es, eax

    // Jump to kernel address space
    movabs rax, offset upper_memory
    jmp rax

upper_memory:
    mov rax, KERNEL_OFFSET
    add rsp, rax

    // Remove identity mapping
    mov rax, 0
    movabs [BootP4], rax

    // Update page tables
    mov rax, cr3
    mov cr3, rax

    // Reload GDT
    movabs rax, offset GDTp
    lgdt [rax]
    mov rax, 0x0
    mov ss, rax
    mov ds, rax
    mov es, rax

    // Reload CS
    movabs rax, offset .reload_cs
    pushq 0x8
    push rax
    retfq
.reload_cs:

// Jump to kernel main function
.extern kmain
    movabs rax, offset kmain
    call rax

    hlt
    jmp $
