#ifndef FONT_H
#define FONT_H

#include "../cpu/types.h"

extern char _binary_src_drivers_gr8x16_psf_start;
extern char _binary_src_drivers_gr8x16_psf_end;

u32 *unicode;

#define PSF1_FONT_MAGIC 0x0436

typedef struct
{
    u16 magic;
    u8 fontMode;
    u8 characterSize;
} PSF1_Header;

#define PSF_FONT_MAGIC 0x864ab572

typedef struct
{
    u32 magic;
    u32 version;
    u32 headersize;
    u32 flags;
    u32 numglyph;
    u32 bytesperglyph;
    u32 height;
    u32 width;
} PSF_Font;

void psf_init();

#endif
