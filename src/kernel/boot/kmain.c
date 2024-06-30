#include <memory.h>
#include <vga.h>
#include <debug.h>

void kmain()
{
    vga_init();

    debug("Hello from debug print\n");
    debug("A number: %d\n", 14325);

    debug_info("Some info\n");
    debug_ok("Worked well!\n");
    debug_warning("Bee careful :D\n");
    debug_error("Smth went wrong ;(((((\n");

    PANIC("Reached the end of kernel main function!\n");
    for (;;)
        ;
}
