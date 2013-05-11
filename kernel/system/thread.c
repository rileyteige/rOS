#include <list.h>
#include <system.h>
#include <thread.h>

extern list_t* ready_queue;
extern list_t* kill_queue;
extern thread_t* current_thread;

thread_t* thread_create(int id)
{
    thread_t* t = (thread_t*)kmalloc(sizeof(thread_t));
    t->id = id;
    return t;
}

void thread_start_execution()
{
    current_thread->start_func();
    thread_finish(current_thread);
}

void thread_start(thread_t* t, void (*func)())
{
    uint32_t esp, ebp;
    asm volatile ("mov %%esp, %0" : "=r" (esp));
    asm volatile ("mov %%ebp, %0" : "=r" (ebp));
    
    t->kernel_stack = kmalloc(KERNEL_STACK_SIZE);
    t->context.esp = (uint32_t)t->kernel_stack + KERNEL_STACK_SIZE;
    t->context.ebp = t->context.esp;
    t->context.eip = (uint32_t)thread_start_execution;
    t->start_func = func;
    t->finished = 0;
    list_enqueue(ready_queue, t);
}

void thread_yield()
{
    task_switch();
}

void thread_finish(thread_t* t)
{
    t->finished = 1;
    kfree(t->kernel_stack);
    list_enqueue(kill_queue, t);
    task_switch();
}
