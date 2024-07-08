#include <keyboard.h>
#include <debug.h>

registers *handle_keyboard(registers *s)
{
    debug("Handle kb\n");
    return s;
}

void keyboard_init()
{
    bind_interrupt(INT_KB, handle_keyboard);
}
