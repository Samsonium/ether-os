#include "../include/timer.h"
#include "../include/isr.h"
#include "../include/ports.h"
#include "../include/function.h"

u32 tick = 0;

static void timer_callback(registers_t regs)
{
    tick++;
    UNUSED(regs);
}

void init_timer(u32 freq)
{
    register_interrupt_handler(IRQ0, timer_callback);

    // Get the PIT - hw clock at 1193180 Hz
    u32 divisor = 1193180 / freq;
    u8 low = (u8)(divisor & 0xFF);
    u8 high = (u8)((divisor >> 8) & 0xFF);

    // Send command
    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
