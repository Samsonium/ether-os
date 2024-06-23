#ifndef IDT_H
#define IDT_H

#define IDT_LENGTH 64
#define IDT_MAX_HANDLERS 64

#include <stdint.h>
#include "state.h"
#include "idt_entry.h"

void idt_init();
void idt_set(uint8_t i, uint32_t (*handler)(int_state_t *state), bool user_int);
void idt_unset(uint8_t i);

void idt_attach_int_handler(uint8_t int_number, bool (*handler)(int_state_t *state));
void idt_detach_int_handler(uint8_t int_number, bool (*handler)(int_state_t *state));

void idt_attach_exc_handler(uint8_t exc_number, bool (*handler)(exception_state_t *state));
void idt_detach_exc_handler(uint8_t exc_number, bool (*handler)(exception_state_t *state));

void idt_attach_pm(void (*handler)(int_state_t *state));
void idt_attach_sysm(void (*handler)(int_state_t *state));

void idt_global_int_handler(int_state_t *state);
void idt_global_exc_handler(int_state_t *state);

void idt_soft_int_handler();
bool idt_sys_int_handler(int_state_t *state);

// Exceptions
extern uint32_t idt_exc0();
extern uint32_t idt_exc1();
extern uint32_t idt_exc2();
extern uint32_t idt_exc3();
extern uint32_t idt_exc4();
extern uint32_t idt_exc5();
extern uint32_t idt_exc6();
extern uint32_t idt_exc7();
extern uint32_t idt_exc8();
extern uint32_t idt_exc9();
extern uint32_t idt_exc10();
extern uint32_t idt_exc11();
extern uint32_t idt_exc12();
extern uint32_t idt_exc13();
extern uint32_t idt_exc14();
extern uint32_t idt_exc15();
extern uint32_t idt_exc16();
extern uint32_t idt_exc17();
extern uint32_t idt_exc18();
extern uint32_t idt_exc19();
extern uint32_t idt_exc20();
extern uint32_t idt_exc21();
extern uint32_t idt_exc22();
extern uint32_t idt_exc23();
extern uint32_t idt_exc24();
extern uint32_t idt_exc25();
extern uint32_t idt_exc26();
extern uint32_t idt_exc27();
extern uint32_t idt_exc28();
extern uint32_t idt_exc29();
extern uint32_t idt_exc30();
extern uint32_t idt_exc31();

// Hardware interrupts
extern uint32_t idt_int32();
extern uint32_t idt_int33();
extern uint32_t idt_int34();
extern uint32_t idt_int35();
extern uint32_t idt_int36();
extern uint32_t idt_int37();
extern uint32_t idt_int38();
extern uint32_t idt_int39();
extern uint32_t idt_int40();
extern uint32_t idt_int41();
extern uint32_t idt_int42();
extern uint32_t idt_int43();
extern uint32_t idt_int44();
extern uint32_t idt_int45();
extern uint32_t idt_int46();
extern uint32_t idt_int47();

// Software interrupts
extern uint32_t idt_int50();

#endif
