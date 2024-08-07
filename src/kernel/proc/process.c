#include <process.h>
#include <scheduler.h>
#include <memory.h>
#include <cpu.h>

struct swtch_stack
{
    uint64_t RBP;
    uint64_t RBX;
    uint64_t R12;
    uint64_t R13;
    uint64_t R14;
    uint64_t R15;
    uint64_t isr_return_arg;
    uint64_t RBP2;
    uint64_t ret;
    registers r;
} __attribute__((packed));

void procmm_init(struct process *p);
registers *proc_pagefault(registers *r);

#define csched cpu->scheduler
#define cproc cpu->process

uint64_t next_pid = 1;
struct process *new_process(void (*function)(void))
{
    if (next_pid == 1)
        bind_interrupt(14, proc_pagefault);

    struct process *proc = P2V(pmm_calloc());
    proc->pid = next_pid++;
    proc->stack_ptr = incptr(proc, PAGE_SIZE - sizeof(struct swtch_stack));
    procmm_init(proc);

    struct swtch_stack *stk = proc->stack_ptr;
    stk->RBP = (uint64_t)&stk->RBP2;

    stk->ret = (uint64_t)isr_return;
    stk->isr_return_arg = (uint64_t)&stk->r;

    stk->r.rip = (uint64_t)function;
    stk->r.cs = 0x10 | 3;
    stk->r.ss = 0x18 | 3;
    stk->r.rflags = 3 << 12;
    stk->r.rsp = KERNEL_OFFSET;

    return proc;
}

void yield()
{
    switch_stack(&cproc->stack_ptr, &csched->stack_ptr);
}

void scheduler()
{
    while (1)
    {
        struct process *new = 0;
        while (!(new = scheduler_next()))
            ;

        cproc = new;
        write_cr3(new->P4);
        interrupt_stack(incptr(new, PAGE_SIZE));
        switch_stack(&csched->stack_ptr, &new->stack_ptr);

        ready(cproc);
        cproc = 0;
    }
}

void start_scheduler()
{
    struct process *sched = csched = P2V(pmm_calloc());
    sched->pid = (uint64_t)-1;
    sched->stack_ptr = incptr(sched, PAGE_SIZE - sizeof(struct swtch_stack) + sizeof(registers));
    sched->P4 = kernel_P4;

    struct swtch_stack *stk = sched->stack_ptr;
    stk->RBP = (uint64_t)&stk->RBP2;

    stk->ret = (uint64_t)scheduler;

    uint64_t stack;
    switch_stack(&stack, &sched->stack_ptr);
}
