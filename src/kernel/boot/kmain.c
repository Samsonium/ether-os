#include <memory.h>
#include <vga.h>
#include <debug.h>
#include <multiboot.h>
#include <cpu.h>
#include <interrupts.h>
#include <thread.h>
#include <scheduler.h>

void thread_function()
{
    int thread_id = thread()->tid;

    while (1)
    {
        debug("Thread %d\n", thread_id);
        yield();
    }
}

registers *divbyzero(registers *r)
{
    debug_error("Divide by zero error!\n");
    debug_print_registers(r);
    for (;;)
        ;
}

void kmain(uint64_t multiboot_magic, void *multiboot_data)
{
    vga_init();
    debug_info("Kernel loaded\n");

    multiboot_init(multiboot_magic, P2V(multiboot_data));
    debug_info("Kernel was loaded with CL \"%s\", by <%s>\n", kboot_data.commandline, kboot_data.bootloader);

    memory_init();
    cpu_init();

    debug_ok("Boot process complete!\n");

    ready(new_thread(thread_function));
    ready(new_thread(thread_function));
    ready(new_thread(thread_function));

    start_scheduler();

    PANIC("Reached the end of kernel main function!\n");
    for (;;)
        ;
}
