#ifndef __THREAD_H
#define __THREAD_H

#include <context.h>

/* 16KB Kernel Stack */
#define KERNEL_STACK_SIZE 4 * KB

typedef struct {
    context_t context;
    int id, finished;
    void (*start_func)();
    void* kernel_stack;
} thread_t;

thread_t* thread_create();

void thread_start(thread_t* t, void (*func)());
void thread_yield();

void thread_finish();

#endif // __THREAD_H
