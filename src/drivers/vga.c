#include "vga.h"
#include "../cpu/ports.h"

void write_registers(u8 *registers)
{
    // misc
    port_byte_out(VGA_MISC, *(registers++));

    // seq
    for (int i = 0; i < 5; i++)
    {
        port_byte_out(VGA_SEQ_INDEX, i);
        port_byte_out(VGA_SEQ_DATA, *(registers++));
    }

    // CRTC
    port_byte_out(VGA_CRTC_INDEX, 0x03);
    port_byte_out(VGA_CRTC_DATA, port_byte_in(VGA_CRTC_DATA) | 0x80);
    port_byte_out(VGA_CRTC_INDEX, 0x11);
    port_byte_out(VGA_CRTC_DATA, port_byte_in(VGA_CRTC_DATA) & ~0x80);

    registers[0x03] = registers[0x03] | 0x80;
    registers[0x11] = registers[0x11] & ~0x80;

    for (int i = 0; i < 25; i++)
    {
        port_byte_out(VGA_CRTC_INDEX, i);
        port_byte_out(VGA_CRTC_DATA, *(registers++));
    }

    // GC
    for (int i = 0; i < 9; i++)
    {
        port_byte_out(VGA_GC_INDEX, i);
        port_byte_out(VGA_GC_DATA, *(registers++));
    }

    // AC
    for (int i = 0; i < 21; i++)
    {
        (void *)port_byte_in(VGA_AC_RESET);
        port_byte_out(VGA_AC_INDEX, i);
        port_byte_out(VGA_AC_WRITE, *(registers++));
    }

    port_byte_in(VGA_AC_READ);
    port_byte_out(VGA_AC_INDEX, 0x20);
}

int vga_supports_mode(u32 w, u32 h, u32 bbp)
{
    return w == 320 && h == 200 && bbp == 8;
}

int vga_set_mode(u32 w, u32 h, u32 dpp)
{
    if (vga_supports_mode(w, h, dpp) == 0)
        return 1;

    unsigned char g_320x200x256[] =
        {
            /* MISC */
            0x63,
            /* SEQ */
            0x03, 0x01, 0x0F, 0x00, 0x0E,
            /* CRTC */
            0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
            0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
            0xFF,
            /* GC */
            0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
            0xFF,
            /* AC */
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            0x41, 0x00, 0x0F, 0x00, 0x00};

    write_registers(g_320x200x256);
    return 0;
}

u8 *get_framebuffer_segment()
{
    port_byte_out(VGA_GC_INDEX, 0x06);
    u8 seg_num = ((port_byte_in(VGA_GC_DATA) >> 2) & 0x03);

    switch (seg_num)
    {
    default:
    case 0:
        return (u8 *)0x00000;
    case 1:
        return (u8 *)0xA0000;
    case 2:
        return (u8 *)0xB0000;
    case 3:
        return (u8 *)0xB8000;
    }
}

u8 vga_get_color_index(u8 r, u8 g, u8 b)
{
    if (r == 0x00 && g == 0x00 && b == 0xA8)
        return 0x01;
}

void vga_putpixel(u32 x, u32 y, u8 color_index)
{
    u8 *pixelAddress = get_framebuffer_segment() + 320 * y + x;
    *pixelAddress = color_index;
}

void vga_putpixel_c(u32 x, u32 y, u8 r, u8 g, u8 b)
{
    vga_putpixel(x, y, vga_get_color_index(r, g, b));
}
