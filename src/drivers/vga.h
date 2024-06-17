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

int vga_supports_mode(u32 w, u32 h, u32 bbp);
int vga_set_mode(u32 w, u32 h, u32 dpp);

void write_registers(u8 *registers);
u8 *get_framebuffer_segment();

u8 vga_get_color_index(u8 r, u8 g, u8 b);
void vga_putpixel(u32 x, u32 y, u8 color_index);
void vga_putpixel_c(u32 x, u32 y, u8 r, u8 g, u8 b);

#endif
