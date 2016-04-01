#include "job_deque.h"
#include "../common/likely.h"

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

#define SEGMENT_NUM 32

struct segment {
    struct task task_block[SEGMENT_NUM];
    uint32_t top, bottom;
    struct segment *next;
    struct segment *prev;
};

static struct segment *create_segment() {
    struct segment *s = malloc(sizeof(*s));
    s->next = s->prev = NULL;
    s->bottom = s->top = 0;
    return s;
}

static void free_segment(struct segment *seg) {
    free(seg);
}



static int pop_seg(struct segment *topseg, struct task *towrite) {
    if unlikely(topseg->top == topseg->bottom) {
        return 0;
    }
    *towrite = topseg->task_block[--topseg->top];
    return 1;
}

static int pop_from_top_seg(struct job_deque *deque, struct task *write_to) {
    if (deque->top == deque->bottom ||
        (deque->bottom->next == deque->top && deque->top->top == 0)) {
        return 0;
    }

    struct segment *old_top = deque->top;
    deque->top = deque->top->prev;
    free_segment(old_top);
    deque->top->next = NULL; // needed?
    deque->top->top = SEGMENT_NUM - 1;
    *write_to = deque->top->task_block[SEGMENT_NUM - 1];
    return 1;
}

int pop_top(struct job_deque *deque, struct task *write_to) {
    if unlikely(!pop_seg(deque->top, write_to)) {
        return pop_from_top_seg(deque, write_to);
    }
    return 1;
}



void push_top(struct job_deque *deque, struct task new) {
    if likely(deque->top->top != SEGMENT_NUM) {
        deque->top->task_block[deque->top->top++] = new;
    }
    else {
        struct segment *new_top = create_segment();
        deque->top->next = new_top;
        new_top->prev = deque->top;
        new_top->task_block[0] = new;
        new_top->top = 1;
        deque->top = new_top;
    }
}
