#pragma once

#include <interrupts.h>

#define INT_KB 33

void keyboard_init();
registers *handle_keyboard(registers *s);
