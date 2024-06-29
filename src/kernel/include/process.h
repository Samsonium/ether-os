#pragma once

// #include <cpu.h>
#include <queue.h>
#include <stdint.h>
#include <scheduler.h>
#include <interrupts.h>

struct process
{
    uint64_t pid;
    void *stack_ptr;
    uint64_t state;

    uint64_t P4;
    uint64_t brk;
    uint64_t stack;

    QUEUE_SPOT(runQ);
};

struct process *new_process(void (*function)(void));
void yield();
void start_scheduler();

void switch_stack(void *, void *);

uint64_t procmm_brk(struct process *, void *);
