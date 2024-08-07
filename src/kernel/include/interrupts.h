#pragma once

#include <stdint.h>

typedef struct
{
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;

    uint64_t int_no;
    uint64_t err_code;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} registers;

#define debug_print_registers(r)                                                        \
    debug("RAX=%016x RBX=%016x RCX=%016x RDX=%016x\n", r->rax, r->rbx, r->rcx, r->rdx); \
    debug("RSI=%016x RDI=%016x RBP=%016x RSP=%016x\n", r->rsi, r->rdi, r->rbp, r->rsp); \
    debug("R8 =%016x R9 =%016x R10=%016x R11=%016x\n", r->r8, r->r9, r->r10, r->r11);   \
    debug("R12=%016x R13=%016x R14=%016x R15=%016x\n", r->r12, r->r13, r->r14, r->r15); \
    debug("RIP=%016x RFL=%08x\n", r->rip, r->rflags);                                   \
    debug("CS=%016x SS=%016x\n", r->cs, r->ss);                                         \
    debug("CR0=%08x CR2=%016x CR3=%016x CR4=%08x\n", read_cr0(), read_cr2(), read_cr3(), read_cr4());

typedef registers *(*int_handler_t)(registers *);

void interrupt_init();
int_handler_t bind_interrupt(uint32_t num, int_handler_t fn);
void isr_return(registers *);
