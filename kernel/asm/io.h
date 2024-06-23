#ifndef IO_H
#define IO_H

#include <stdint.h>

uint8_t byte_in(uint16_t port);
void byte_out(uint16_t port, uint8_t value);

uint16_t word_in(uint16_t port);
void word_out(uint16_t port, uint16_t value);

void io_interrupts_enable();
void io_interrupts_disable();

void io_wait();

#endif
