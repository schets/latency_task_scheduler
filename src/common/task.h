#ifndef LATENCY_JOBQUEUE_TASK_H
#define LATENCY_JOBQUEUE_TASK_H

struct context;

struct task {
    void (*operation)(void *, struct context *);
    void *data;
};

#endif
