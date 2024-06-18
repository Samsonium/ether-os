#include "../cpu/isr.h"
#include "../cpu/paging.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "cmd.h"

void kmain()
{
	isr_install();
	irq_install();
	init_paging();

	clear_screen();
	kprint("Welcome to EtherOS! Write something. Type \"END\" to halt the CPU", get_color(L_GREEN, BLACK));
	kprint("\n> ", get_color(L_CYAN, BLACK));
}

void user_input(char *input)
{
	cmd_exec(input);
}
