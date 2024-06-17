#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "cmd.h"

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
	cmd_exec(input);
}
