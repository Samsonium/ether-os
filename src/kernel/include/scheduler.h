#pragma once

#include <queue.h>

#define runQ readyQ, readyQ_next, struct process

extern QUEUE_DECLARE(runQ);

void ready(struct process *proc);
struct process *scheduler_next();
