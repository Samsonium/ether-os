#include <scheduler.h>
#include <queue.h>
#include <process.h>

QUEUE_DEFINE(runQ);

void ready(struct process *th)
{
    queue_add(runQ, th);
}

struct process *scheduler_next()
{
    return queue_pop(runQ);
}
