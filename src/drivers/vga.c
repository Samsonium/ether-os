#include "vga.h"
#include "screen.h"
#include "../cpu/ports.h"

void draw_rectangle(int x, int y, int width, int height, u16 color);
void vga_clear_screen();
void vga_setpix(int x, int y, u16 color);

u8 g_320x200x256[] = {
    // MISC
    0x63,

    // SEQ
    0x03, 0x01, 0x0F, 0x00, 0x0E,

    // CRTC
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,

    // GC
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,

    // AC
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00, 0x00};

void vga_test()
{
    kprint("Attempting to switch mode...", get_color(YELLOW, BLACK));

    write_regs(g_320x200x256);
    vga_clear_screen();

    draw_rectangle(150, 10, 100, 50, GREEN);

    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            vga_setpix(i, 50 + j, i);
        }
    }
}

void draw_rectangle(int x, int y, int width, int height, u16 color)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            vga_setpix(x + i, y + j, color);
        }
    }
}

void vga_clear_screen()
{
    draw_rectangle(0, 0, 320, 200, BLACK);
}

void vga_setpix(int x, int y, u16 color)
{
    u16 offset = x + 320 * y;
    u16 *vga = (u16 *)VGA_ADDRESS;
    vga[offset] = color;
}

void write_regs(u8 *regs)
{
    unsigned i;

    // Write MISC reg
    port_byte_out(VGA_MISC_WRITE, *regs);
    regs++;

    // Write SEQ regs
    for (i = 0; i < VGA_NUM_SEQ_REGS; i++)
    {
        port_byte_out(VGA_SEQ_INDEX, i);
        port_byte_out(VGA_SEQ_DATA, *regs);
        regs++;
    }

    // Unlock CRTC regs
    port_byte_out(VGA_CRTC_INDEX, 0x03);
    port_byte_out(VGA_CRTC_DATA, port_byte_in(VGA_CRTC_DATA) | 0x80);
    port_byte_out(VGA_CRTC_INDEX, 0x11);
    port_byte_out(VGA_CRTC_DATA, port_byte_in(VGA_CRTC_DATA) & -0x80);

    // Make sure CRTC is unlocked
    regs[0x03] |= 0x80;
    regs[0x11] &= -0x80;

    // Write CRTC regs
    for (i = 0; i < VGA_NUM_CRTC_REGS; i++)
    {
        port_byte_out(VGA_CRTC_INDEX, i);
        port_byte_out(VGA_CRTC_DATA, *regs);
        regs++;
    }

    // Write graphics controller regs
    for (i = 0; i < VGA_NUM_GC_REGS; i++)
    {
        port_byte_out(VGA_GC_INDEX, i);
        port_byte_out(VGA_GC_DATA, *regs);
        regs++;
    }

    // Write attribute controller regs
    for (i = 0; i < VGA_NUM_AC_REGS; i++)
    {
        (void)port_byte_in(VGA_INSTAT_READ);
        port_byte_out(VGA_AC_INDEX, i);
        port_byte_out(VGA_AC_WRITE, *regs);
        regs++;
    }

    // Lock 16-color paletter and unblank display
    (void)port_byte_in(VGA_INSTAT_READ);
    port_byte_out(VGA_AC_INDEX, 0x20);
}
