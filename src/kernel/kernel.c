#include "../cpu/isr.h"
#include "../cpu/paging.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "cmd.h"

char logo[] = {" ________  _  __                    ___    ______   \n|_   __  |/ |[  |                 .'   `..' ____ \\  \n  | |_ \\_`| |-| |--. .---. _ .--./  .-.  | (___ \\_| \n  |  _| _ | | | .-. / /__\\[ `/'`\\| |   | |_.____`.  \n _| |__/ || |,| | | | \\__.,| |   \\  `-'  | \\____) | \n|________|\\__[___]|__'.__.[___]   `.___.' \\______.' \n,                                                    "};

void kmain()
{
	isr_install();
	irq_install();
	init_paging();

	clear_screen();

	kprint(logo, get_color(L_GREEN, BLACK));
	kprint("\n", get_color(BLACK, BLACK));

	kprint("Welcome to EtherOS! Write something. Type \"END\" to halt the CPU", get_color(WHITE, BLACK));
	kprint("\n> ", get_color(L_CYAN, BLACK));
}

void user_input(char *input)
{
	cmd_exec(input);
}
