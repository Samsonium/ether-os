#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include "state.h"

typedef struct interrupt_handler
{
    uint8_t int_number;
    bool (*handler)(int_state_t *state);
} interrupt_handler_t;

#endif
