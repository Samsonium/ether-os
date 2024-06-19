#include "../include/screen.h"
#include "../include/ports.h"
#include "../include/mem.h"

// Declare private kernel API functions
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/////////////////////////////////
// PUBLIC KERNEL API FUNCTIONS //
/////////////////////////////////

/// Print message on specified position
void kprint_at(char *msg, int col, int row, char color)
{
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else
    {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    if (!color)
        color = get_color(WHITE, BLACK);

    // loop through message and print it
    int i = 0;
    while (msg[i] != 0)
    {
        offset = print_char(msg[i++], col, row, color);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

/// Print message on actual position
void kprint(char *msg, char color)
{
    kprint_at(msg, -1, -1, color);
}

/// Removes last visible character in input string
void kprint_backspace()
{
    int offset = get_cursor_offset() - 2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(' ', col, row, get_color(BLACK, BLACK));

    set_cursor_offset(offset);
}

/// Get char attribute by fg and bg
char get_color(enum Color fg, enum Color bg)
{
    return (bg << 4) | fg;
}

//////////////////////////////////
// PRIVATE KERNEL API FUNCTIONS //
//////////////////////////////////

/// Print function with direct access to video memory
int print_char(char c, int col, int row, char attr)
{
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS)
    {
        vidmem[2 * (MAX_COLS) * (MAX_ROWS)-2] = 'E';
        vidmem[2 * (MAX_COLS) * (MAX_ROWS)-1] = get_color(RED, WHITE);
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else
        offset = get_cursor_offset();

    if (c == '\n')
    {
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    }
    else
    {
        vidmem[offset] = c;
        vidmem[offset + 1] = attr;
        offset += 2;
    }

    /* Check if the offset is over screen size and scroll */
    if (offset >= MAX_ROWS * MAX_COLS * 2)
    {
        int i;
        for (i = 1; i < MAX_ROWS; i++)
            memory_copy(get_offset(0, i) + ((char *)VIDEO_ADDRESS),
                        get_offset(0, i - 1) + ((char *)VIDEO_ADDRESS),
                        MAX_COLS * 2);

        /* Blank last line */
        char *last_line = get_offset(0, MAX_ROWS - 1) + ((char *)VIDEO_ADDRESS);
        for (i = 0; i < MAX_COLS * 2; i++)
            last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }

    set_cursor_offset(offset);
    return offset;
}

int get_cursor_offset()
{
    port_byte_out(REG_SCREEN_CTRL, 14);

    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);

    offset += port_byte_in(REG_SCREEN_DATA);

    return offset * 2; // position * size of character cell
}

void set_cursor_offset(int offset)
{
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xFF));
}

void clear_screen()
{
    int screen_size = MAX_COLS * MAX_ROWS;

    int i;
    char *screen = (char *)VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++)
    {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = get_color(BLACK, BLACK);
    }

    set_cursor_offset(get_offset(0, 0));
}

int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2; }
