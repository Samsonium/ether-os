#include <cpu.h>
#include <interrupts.h>
#include <memory.h>
#include <debug.h>

void gdt_init(struct cpu *c);

struct cpu __seg_gs *cpu = 0;

void cpu_init()
{
    interrupt_init();
    gdt_init(cpu->cpu);
}
