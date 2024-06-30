#include <memory.h>
#include <vga.h>
#include <debug.h>
#include <multiboot.h>
#include <cpu.h>
#include <interrupts.h>
#include <process.h>
#include <scheduler.h>

void thread_function()
{
    while (1)
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

    struct process *p1 = new_process((void (*)(void))0x10000);
    uint64_t page = pmm_alloc();
    vmm_set_page(p1->P4, 0x10000, page, PAGE_WRITE | PAGE_PRESENT | PAGE_USER);
    memcpy(P2V(page), (void *)(uintptr_t)thread_function, PAGE_SIZE);

    ready(p1);

    debug_ok("Boot process complete!\n");

    start_scheduler();

    PANIC("Reached the end of kernel main function!\n");
    for (;;)
        ;
}
