#include <cpu.h>
#include <interrupts.h>
#include <memory.h>
#include <debug.h>

void gdt_init(cpu_t *c);

cpu_t __seg_gs *cpu = 0;

void cpu_init()
{
    cpu_t *c = P2V(pmm_alloc());
    c->cpu = c;
    write_msr(KERNEL_GS_BASE, (uint64_t)c);
    asm("swapgs");

    interrupt_init();
    gdt_init(cpu->cpu);
}
