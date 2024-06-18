#include "../cpu/isr.h"
#include "../cpu/paging.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "cmd.h"

const char *logo[] = {" ________  _  __                    ___    ______   ",
					  "|_   __  |/ |[  |                 .'   `..' ____ \\  ",
					  "  | |_ \\_`| |-| |--. .---. _ .--./  .-.  | (___ \\_| ",
					  "  |  _| _ | | | .-. / /__\\[ `/'`\\| |   | |_.____`.  ",
					  " _| |__/ || |,| | | | \\__.,| |   \\  `-'  | \\____) | ",
					  "|________|\\__[___]|__'.__.[___]   `.___.' \\______.' ",
					  "                                                    "};

void kmain()
{
	isr_install();
	irq_install();
	init_paging();

	clear_screen();

	for (int i = 0; i < 7; i++)
	{
		kprint(logo[i], get_color(L_GREEN, BLACK));
		kprint("\n", 0x00);
	}

	kprint("Welcome to EtherOS! Write something. Type \"END\" to halt the CPU", get_color(WHITE, BLACK));
	kprint("\n> ", get_color(L_CYAN, BLACK));
}

void user_input(char *input)
{
	cmd_exec(input);
}
