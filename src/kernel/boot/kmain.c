#include <memory.h>
#include <vga.h>
#include <debug.h>
#include <multiboot.h>

void kmain(uint64_t multiboot_magic, void *multiboot_data)
{
    vga_init();
    debug_info("Kernel loaded\n");

    multiboot_init(multiboot_magic, P2V(multiboot_data));
    debug_info("Kernel was loaded with CL \"%s\", by <%s>\n", kboot_data.commandline, kboot_data.bootloader);

    debug_ok("Boot process complete!\n");

    PANIC("Reached the end of kernel main function!\n");
    for (;;)
        ;
}
