#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C

#define SHIFT_DOWN 0x2A
#define SHIFT_UP 0xAA

static char key_buffer[256];
int shift_mod = 0;

#define SC_MAX 57

const char *sc_name[] = {
    "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab",
    "q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
    "[", "]", "Enter", "Lctrl", "a", "s", "d", "f",
    "g", "h", "j", "k", "l", ";", "'", "`", "LShift",
    "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".",
    "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const char sc_ascii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '?', '\0', 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', '\0', '\0',
    '\0', ' '};

const char sc_mod_ascii[] = {
    '?', 'E', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '?', ' ',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}', '\n', '?', 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    '"', '~', '?', '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', '\0', '*',
    '\0', ' '};

static void keyboard_callback(registers_t regs)
{
    u8 scancode = port_byte_in(0x60);

    // Check for 'Shift' modifier
    if (scancode == SHIFT_DOWN)
    {
        shift_mod = 1;
        return;
    }
    else if (scancode == SHIFT_UP)
    {
        shift_mod = 0;
        return;
    }

    // Check for modifiers

    // Check for out of keydown range
    if (scancode > SC_MAX)
        return;

    // Check for scancodes
    if (scancode == BACKSPACE)
    {
        if (strlen(key_buffer) < 1)
            return;

        backspace(key_buffer);
        kprint_backspace();
    }
    else if (scancode == ENTER)
    {
        kprint("\n", 0xFF);
        user_input(key_buffer);
        key_buffer[0] = '\0';
    }
    else
    {
        char letter = shift_mod
                          ? sc_mod_ascii[(int)scancode]
                          : sc_ascii[(int)scancode];
        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        kprint(str, 0x0F);
    }

    UNUSED(regs);
}

void init_kb()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}
