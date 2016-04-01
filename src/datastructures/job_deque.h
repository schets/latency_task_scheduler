#ifndef JOB_DEQUE_H
#define JOB_DEQUE_H

#include "../common/task.h"
#include <stdint.h>

struct segment;


// Single threaded local job deque used by each worker
struct job_deque {
    struct segment *top;
    struct segment *bottom;
    struct segment *seg_cache;
};

// Inline these at some point...
// Or make more effecient w.r.t. calling convention
int pop_top(struct job_deque *deque, struct task *into);
void push_top(struct job_deque *deque, struct task topush);
int steal(struct job_deque *deque, struct task *into);

#endif
