#ifndef SCREEN_H
#define SCREEN_H

#include "../cpu/types.h"

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

// Screen I/O ports
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

enum Color
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    L_GREY = 7,
    D_GREY = 8,
    L_BLUE = 9,
    L_GREEN = 10,
    L_CYAN = 11,
    L_RED = 12,
    L_MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

void clear_screen();
void kprint_at(char *msg, int col, int row, char color);
void kprint(char *msg, char color);
void kprint_backspace();

char get_color(enum Color fg, enum Color bg);

#endif
