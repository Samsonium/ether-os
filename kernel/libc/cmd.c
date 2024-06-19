#include "../include/cmd.h"
#include "../include/string.h"
#include "../include/screen.h"
#include "../include/vga.h"
#include "../include/kheap.h"

void cmd_help()
{
    char color = get_color(WHITE, BLACK);

    kprint("Help:\n", color);
    kprint(" - end - begin cpu halt. This action will stop OS execution\n", color);
    kprint(" - page - allocate some memory and display page and physcial address\n", color);
    kprint(" - vga - enter VGA mode. Reboot to return back\n", color);
    kprint(" - clear - clears the screen\n", color);
    kprint(" - help - list available commands\n\n", color);
}

void cmd_end()
{
    kprint("Stopping the CPU. Bye-bye!\n", get_color(BLACK, WHITE));
    asm volatile("hlt");
}

void cmd_page()
{
    u32 page = kmalloc(10);

    char page_str[16] = "";
    hex_to_ascii(page, page_str);

    kprint("Page: ", get_color(WHITE, BLACK));
    kprint(page_str, get_color(L_GREEN, BLACK));
    kprint("\n", 0x00);
}

void cmd_vga_test()
{
    int err_code = vga_set_mode(320, 200, 8);
    if (err_code != 0)
    {
        kprint("Cannot init VGA\n", get_color(RED, BLACK));
        return;
    }

    vga_welcome();
}

void cmd_clear()
{
    clear_screen();
}

void cmd_exec(char *input)
{
    if (strcmp(input, "help") == 0)
        cmd_help();
    else if (strcmp(input, "end") == 0)
    {
        cmd_end();
        return;
    }
    else if (strcmp(input, "page") == 0)
        cmd_page();
    else if (strcmp(input, "vga") == 0)
        cmd_vga_test();
    else if (strcmp(input, "clear") == 0)
        cmd_clear();
    else
    {
        kprint("Unknown command. Type HELP to list available commands\n", get_color(RED, BLACK));
    }

    kprint("> ", get_color(L_CYAN, BLACK));
}
