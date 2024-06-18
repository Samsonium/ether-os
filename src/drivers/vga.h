#ifndef VGA_H
#define VGA_H

#include "../cpu/types.h"

#define VGA_MISC 0x3C2
#define VGA_CRTC_INDEX 0x3D4
#define VGA_CRTC_DATA 0x3D5
#define VGA_SEQ_INDEX 0x3C4
#define VGA_SEQ_DATA 0x3C5
#define VGA_GC_INDEX 0x3CE
#define VGA_GC_DATA 0x3CF
#define VGA_AC_INDEX 0x3C0
#define VGA_AC_READ 0x3C1
#define VGA_AC_WRITE 0x3C0
#define VGA_AC_RESET 0x3DA
#define VGA_PALETTE_MASK 0x3C6
#define VGA_PALETTE_READ 0x3C7
#define VGA_PALETTE_WRITE 0x3C8
#define VGA_PALETTE_DATA 0x3C9

typedef enum VGAColor
{
    VGA_BLACK = 0x0,
    VGA_DARK_BLUE = 0x1,
    VGA_GREEN = 0x2,
    VGA_TEAL = 0x3,
    VGA_RED = 0x4,
    VGA_PURPLE = 0x5,
    VGA_YELLOW = 0x6,
    VGA_GRAY = 0x7,
    VGA_NAVY = 0x8,
    VGA_BLUE = 0x9,
    VGA_LIGHT_GREEN = 0xA,
    VGA_SKY_BLUE = 0xB,
    VGA_MAROON = 0xC,
    VGA_BRIGHT_PURPLE = 0xD,
    VGA_BEIGE = 0xE,
    VGA_INDIGO = 0xF,
    VGA_WHITE = 0x3F
} color_t;

int vga_supports_mode(u32 w, u32 h, u32 bbp);
int vga_set_mode(u32 w, u32 h, u32 dpp);

void write_registers(u8 *registers);
u8 *get_framebuffer_segment();

void vga_putpixel(u32 x, u32 y, u8 color_index);
void vga_fillrect(u32 x, u32 y, u32 w, u32 h, u8 color_index);
void vga_drawchar(u8 c, u32 x, u32 y, u8 color_index);
void vga_drawstr(u8 s[], u32 x, u32 y, u8 color_index);
void vga_welcome();

#endif
