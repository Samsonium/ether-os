#include <memory.h>

void clear_screen()
{
    unsigned char *vmem = P2V(0xB8000);
    for (int i = 0; i < 80 * 24 * 2; i++)
        *vmem++ = 0;
}

void print_string(char *str)
{
    unsigned char *vmem = P2V(0xB8000);
    while (*str)
    {
        *vmem++ = *str++;
        *vmem++ = 0x0F;
    }
}

void kmain()
{
    clear_screen();
    print_string("Hello, World from C!");

    for (;;)
        ;
}
