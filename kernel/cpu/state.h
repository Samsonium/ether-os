#ifndef STATE_H
#define STATE_H

#include <stdint.h>

typedef struct fpu_state
{
    uint32_t control_word;
    uint32_t status_word;
    uint32_t tag_word;
    uint32_t instr_pointer_offset;
    uint32_t instr_pointer_sel;
    uint16_t opcode;
    uint32_t op_pointer_offset;
    uint32_t op_pointer_sel;
    uint8_t registers[80];
} fpu_state_t;

typedef struct registers_state
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_unused;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
} registers_state_t;

typedef struct int_state
{
    registers_state_t reigsters;
    fpu_state_t fpu_state;

    uint32_t int_number;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} int_state_t;

typedef struct exception_state
{
    uint64_t idtr;
    uint64_t gdtr;

    uint32_t gs;
    uint32_t fs;
    uint32_t es;

    uint32_t cr4;
    uint32_t cr3;
    uint32_t cr2;
    uint32_t cr0;

    uint32_t ds;

    registers_state_t registers;
    fpu_state_t fpu_state;

    uint32_t int_number;

    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} exception_state_t;

#endif
