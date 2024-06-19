#include "../include/ports.h"

/// Read a byte from the specified port
unsigned char port_byte_in(u16 port)
{
	unsigned char result;

	__asm__("in %%dx, %%al" : "=a"(result) : "d"(port));

	return result;
}

/// Write out byte to specified port
void port_byte_out(u16 port, u8 data)
{
	__asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}

/// Read word from the specified port
unsigned short port_word_in(u16 port)
{
	unsigned short result;

	__asm__("in %%dx, %%ax" : "=a"(result) : "d"(port));

	return result;
}

/// Write worte to specified port
void port_word_out(u16 port, u8 data)
{
	__asm__("out %%ax, %%dx" : : "a"(data), "d"(port));
}
