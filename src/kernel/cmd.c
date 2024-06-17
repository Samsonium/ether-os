#include "cmd.h"
#include "../libc/string.h"
#include "../drivers/screen.h"
#include "../drivers/vga.h"
#include "../libc/mem.h"

void cmd_help()
{
    char color = get_color(WHITE, BLACK);

    kprint("Help:\n", color);
    kprint(" - END - begin cpu halt\n", color);
    kprint(" - PAGE - allocate some memory\n", color);
    kprint(" - VGA - enter VGA mode\n", color);
    kprint(" - HELP - list available commands\n\n", color);
}

void cmd_end()
{
    kprint("Stopping the CPU. Bye-bye!\n", get_color(BLACK, WHITE));
    asm volatile("hlt");
}

void cmd_page()
{
    u32 phys_addr;
    u32 page = kmalloc(10, 1, &phys_addr);

    char page_str[16] = "";
    hex_to_ascii(page, page_str);

    char phys_str[16] = "";
    hex_to_ascii(phys_addr, phys_str);

    kprint("Page: ", get_color(WHITE, BLACK));
    kprint(page_str, get_color(L_GREEN, BLACK));
    kprint(", physical address: ", get_color(WHITE, BLACK));
    kprint(phys_str, get_color(L_GREEN, BLACK));
    kprint("\n", 0x00);
}

void cmd_vga_test()
{
    vga_test();
}

void cmd_exec(char *input)
{
    if (strcmp(input, "HELP") == 0)
        cmd_help();
    else if (strcmp(input, "END") == 0)
    {
        cmd_end();
        return;
    }
    else if (strcmp(input, "PAGE") == 0)
        cmd_page();
    else if (strcmp(input, "VGA") == 0)
        cmd_vga_test();
    else
    {
        kprint("Unknown command. Type HELP to list available commands\n", get_color(RED, BLACK));
    }

    kprint("> ", get_color(L_CYAN, BLACK));
}