#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Make fg+bg color for VGA entry
static inline uint8_t vga_entry_color(uint8_t fg, uint8_t bg)
{
    return fg | bg << 4;
}

// Create colorized VGA entry
static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

// Get length of the string
size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

// VGA dimensions
static const size_t VGA_W = 80;
static const size_t VGA_H = 25;

// Terminal data
size_t tr;
size_t tc;
uint8_t color;
uint16_t *buffer;

// Initialize terminal
void term_init(void)
{
    tr = 0;
    tc = 0;
    color = vga_entry_color(0x7, 0x0);
    buffer = (uint16_t *)0xB8000;

    size_t x, y;
    for (y = 0; y < VGA_H; y++)
    {
        for (x = 0; x < VGA_W; x++)
        {
            const size_t index = y * VGA_W + x;
            buffer[index] = vga_entry(' ', color);
        }
    }
}

// Put VGA entry at specified position
void term_putat(char c, size_t x, size_t y)
{
    const size_t index = y * VGA_W + x;
    buffer[index] = vga_entry(c, color);
}

// Put character at specified pos
void term_putchar(char c)
{
    if (c == '\n')
    {
        tc = 0;
        tr++;
        return;
    }

    term_putat(c, tc, tr);
    if (++tc == VGA_W)
    {
        tc = 0;
        if (++tr == VGA_H)
            tr = 0;
    }
}

void term_write(const char *data)
{
    size_t i;
    size_t size = strlen(data);
    for (i = 0; i < size; i++)
        term_putchar(data[i]);
}

void kmain(void)
{
    term_init();
    term_write("Hello, kernel world!\n");
    term_write("I'm on line after!!");
}
