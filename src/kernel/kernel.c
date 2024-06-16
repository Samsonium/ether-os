#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"

void kmain()
{
	isr_install();
	irq_install();

	clear_screen();
	kprint("Welcome to SmthOS! Write something. Type \"END\" to halt the CPU", get_color(L_GREEN, BLACK));
	kprint("\n> ", get_color(L_CYAN, BLACK));
}

void user_input(char *input)
{
	if (strcmp(input, "END") == 0)
	{
		kprint("Stopping the CPU. Bye-bye!\n", get_color(BLACK, WHITE));
		asm volatile("hlt");
	}

	char color = get_color(WHITE, BLACK);
	kprint("You said: ", color);
	kprint(input, color);
	kprint("\n> ", get_color(L_CYAN, BLACK));
}
