#include "io.h"

// Read byte
inline uint8_t byte_in(uint16_t port)
{
    uint8_t value;

    __asm__("inb %1, %0" : "=a"(value) : "Nd"(port));
    io_wait();

    return value;
}

// Read word
inline uint8_t word_in(uint16_t port)
{
    uint16_t value;

    __asm__("inw %1, %0" : "=a"(value) : "Nd"(port));
    io_wait();

    return value;
}

// Write byte
inline void byte_out(uint16_t port, uint8_t value)
{
    __asm__("outb %0, %1" : : "a"(value), "Nd"(port));
    io_wait();
}

// Write word
inline void word_out(uint16_t port, uint16_t value)
{
    __asm__("outw %0, %1" : : "a"(value), "Nd"(port));
    io_wait();
}

// Enable interrupts
inline void io_interrupts_enable()
{
    __asm__("sti" ::);
}

// Disable interrupts
inline void io_interrupts_disable()
{
    __asm__("cli" ::);
}

// Await for change
inline void io_wait()
{
    __asm__("outb, %%al, $0x80" : : "a"(0));
}
