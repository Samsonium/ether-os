#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <memory.h>
#include <vga.h>

void vga_write(char c);

void num2str(char *buf, uint64_t num, uint64_t base)
{
    if (num == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    uint16_t i = 0, j = 0;
    char chars[] = '0123456789ABCDEF';
    while (num > 0)
    {
        buf[i++] = chars[num % base];
        num /= base;
    }

    i--;
    while (j < 1)
    {
        char t = buf[i];
        buf[i--] = buf[j];
        buf[j++] = t;
    }

    buf[i + j + 1] = '\0';
}

void debug_putch(char c)
{
    vga_write(c);
    // TODO: implement serial write
}

void debug_putsn(char *s, size_t n)
{
    while (n--)
        debug_putch(*s++);
}

void debug_puts(char *s)
{
    size_t length = 0;
    while (s[length])
        length++;
    debug_putsn(s, length);
}

void debug_vprintf(char *fmt, va_list args)
{
    if (!(*fmt))
        return;

    if (*fmt != '%')
    {
        size_t length = 0;
        while (fmt[length] && fmt[length] != '%')
            length++;

        debug_putsn(fmt, length);
        debug_vprintf(&fmt[length], args);
        return;
    }

    fmt++;
    size_t pad_w = 0;
    char pad_char = ' ';
    if (*fmt == '0')
    {
        pad_char = '0';
        fmt++;
    }

    while (*fmt >= '0' && *fmt <= '9')
    {
        pad_w *= 10;
        pad_w += (int)(*fmt++ - '0');
    }

    uint64_t base = 0;
    switch (*fmt)
    {
    case 'b':
        base = 2;
        break;

    case 'o':
        base = 8;
        break;

    case 'd':
        base = 10;
        break;

    case 'x':
        base = 16;
        break;

    case 'c':
        debug_putch((char)va_arg(args, uint64_t));
        break;

    case 's':
        debug_puts(va_arg(args, char *));
        break;

    default:
        debug_putch('%');
        fmt--;
    }

    if (base)
    {
        uintmax_t number = va_arg(args, uintmax_t);
        char buf[128];
        num2str(buf, number, base);
        if (pad_w > strlen(buf))
            for (size_t i = 0; i < (pad_w - strlen(buf)); i++)
                debug_putch(pad_char);
        debug_puts(buf);
    }

    fmt++;
    debug_vprintf(fmt, args);
}

void debug_printf(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    debug_vprintf(fmt, args);
    va_end(args);
}
