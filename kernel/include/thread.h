#ifndef __THREAD_H
#define __THREAD_H

#include <context.h>
#include <list.h>
#include <types.h>

/* 16KB Kernel Stack */
#define KERNEL_STACK_SIZE 4 * KB

typedef struct {
    context_t context;
    uint8_t id, finished, sleeping;
    void (*start_func)();
    void* kernel_stack;
} __attribute__((packed)) thread_t;

typedef struct {
    thread_t* owner;
    list_t* waiting_queue;
} __attribute__((packed)) mutex_t;

thread_t* thread_create();

void thread_start(thread_t* t, void (*func)());
void thread_yield();
void thread_sleep();

void thread_finish();

mutex_t* mutex_create();
void mutex_lock(mutex_t* m);
void mutex_unlock(mutex_t* m);
void mutex_free(mutex_t* m);

#endif // __THREAD_H
