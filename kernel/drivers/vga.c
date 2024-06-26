#include "../include/vga.h"
#include "../include/font.h"
#include "../include/ports.h"

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
        port_byte_in(VGA_AC_RESET);
        port_byte_out(VGA_AC_INDEX, i);
        port_byte_out(VGA_AC_WRITE, *(registers++));
    }

    port_byte_out(VGA_PALETTE_MASK, 0xFF);
    port_byte_out(VGA_PALETTE_WRITE, 0);

    for (int i = 0; i < 0xFF; i++)
    {
        int r = (i >> 6) & 0x3F;
        int g = (i >> 3) & 0x3F;
        int b = (i >> 0) & 0x3F;

        port_byte_out(VGA_PALETTE_WRITE, i);
        port_byte_out(VGA_PALETTE_DATA, r);
        port_byte_out(VGA_PALETTE_DATA, g);
        port_byte_out(VGA_PALETTE_DATA, b);
    }

    port_byte_out(VGA_PALETTE_WRITE, 0);

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

void vga_putpixel(u32 x, u32 y, u8 color_index)
{
    u8 *pixelAddress = get_framebuffer_segment() + 320 * y + x;
    *pixelAddress = color_index;
}

void vga_fillrect(u32 x, u32 y, u32 w, u32 h, u8 color_index)
{
    for (u32 _y = y; _y < h + y; _y++)
        for (u32 _x = x; _x < w + x; _x++)
            vga_putpixel(_x, _y, color_index);
}

void vga_drawchar(u8 c, u32 x, u32 y, u8 color_index)
{
    u32 cx, cy;
    PSF_Font *font = (PSF_Font *)&_binary___src_drivers_gr8x16_psf_start;
    int bytesperline = (font->width + 7) / 8;
    if (unicode != NULL)
    {
        c = unicode[c];
    }

    u8 *glyph =
        (u8 *)&_binary___src_drivers_gr8x16_psf_start +
        font->headersize +
        (c > 0 && c < font->numglyph ? c : 0) * font->bytesperglyph;

    int offs =
        (cy * font->height * 320) +
        (cx * (font->width + 1) * sizeof(u32));

    u32 line, mask;
    for (cy = 0; cy < font->height; cy++)
    {
        line = offs;
        mask = 1 << (font->width - 1);
        for (cx = 0; cx < font->width; cx++)
        {
            vga_putpixel(cx, cy, *((u32 *)glyph) & mask ? color_index : 0x0);
            mask >>= 1;
            line += sizeof(u32);
        }
        glyph += bytesperline;
        offs += 320;
    }
}

void vga_drawstr(u8 *c, u32 x, u32 y, u8 color_index)
{
    int i = 0;
    while (*c != '\0')
    {
        vga_drawchar(*c, x + x * i, y, color_index);

        i++;
        *c++;
    }
}

void vga_welcome()
{
    for (s32 y = 0; y < 480; y++)
        for (s32 x = 0; x < 320; x++)
            vga_putpixel(x, y, 0x0);

    vga_fillrect(320 / 2 - 32, 200 / 2 - 8, 64, 16, 0x7);
    vga_drawstr("Ether OS", 320 / 2 - 4, 240 / 2 - 1, 0x3F);
}
