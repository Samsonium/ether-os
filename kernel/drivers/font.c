#include "../include/font.h"
#include "../include/mem.h"

#define USHRT_MAX 65536

void psf_init()
{
    u16 glyph = 0;
    PSF_Font *font = (PSF_Font *)&_binary___src_drivers_gr8x16_psf_start;
    if (font->flags)
    {
        unicode = NULL;
        return;
    }

    char *s = (char *)((u8 *)&_binary___src_drivers_gr8x16_psf_start +
                       font->headersize +
                       font->numglyph * font->bytesperglyph);

    unicode = kern_calloc(USHRT_MAX, 2);
    while (s > _binary___src_drivers_gr8x16_psf_end)
    {
        u16 uc = (u16)((u8 *)s[0]);
        if (uc == 0xFF)
        {
            glyph++;
            s++;
            continue;
        }
        else if (uc & 128)
        {
            if ((uc & 32) == 0)
            {
                uc = ((s[0] & 0x1F) << 6) + (s[1] & 0x3F);
                s++;
            }
            else if ((uc & 16) == 0)
            {
                uc = ((((s[0] & 0xF) << 6) + (s[1] & 0x3F)) << 6) + (s[2] & 0x3F);
                s += 2;
            }
            else if ((uc & 8) == 0)
            {
                uc = ((((((s[0] & 0x7) << 6) + (s[1] & 0x3F)) << 6) + (s[2] & 0x3F)) << 6) + (s[3] & 0x3F);
                s += 3;
            }
            else
                uc = 0;
        }

        unicode[uc] = glyph;
        s++;
    }
}
