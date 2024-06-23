#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>
#include <stdbool.h>
#include "state.h"

typedef struct exception
{
    char desc[32];
    uint8_t int_number;
} exception_t;

typedef struct exception_handler
{
    uint8_t exception_number;
    void (*handler)(exception_state_t *state);
} exception_handler_t;

#endif
