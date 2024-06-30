#pragma once

#include <queue.h>

#define runQ readyQ, readyQ_next, struct thread

extern QUEUE_DECLARE(runQ);

void ready(struct thread *th);
struct thread *scheduler_next();
