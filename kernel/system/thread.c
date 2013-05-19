#include <list.h>
#include <system.h>
#include <thread.h>

extern list_t* ready_queue;
extern list_t* kill_queue;
extern thread_t* current_thread;

/*
 * Creates a thread.
 */
thread_t* thread_create(int id)
{
    thread_t* t = (thread_t*)kmalloc(sizeof(thread_t));
    t->id = id;
    t->finished = 0;
    t->sleeping = 0;
    return t;
}

/*
 * The actual entry point for a thread's
 * execution.
 */
void thread_start_execution()
{
    /* Call the user's start func. */
    current_thread->start_func();
    thread_finish(current_thread);
}

/*
 * The exit point of execution for a thread.
 */
void thread_finish(thread_t* t)
{
    t->finished = 1;
    kfree(t->kernel_stack);
    t->kernel_stack = NULL;

    uint8_t old_status = set_interrupts(DISABLED);
    list_enqueue(kill_queue, t);
    set_interrupts(old_status);

    thread_yield();
}

/*
 * Makes a thread ready to execute by pointing it
 * at a starting address for execution and
 * adding it to the ready queue.
 */
void thread_start(thread_t* t, void (*func)())
{
    /* Load our current position on the stack. */
    uint32_t esp, ebp;
    asm volatile ("mov %%esp, %0" : "=r" (esp));
    asm volatile ("mov %%ebp, %0" : "=r" (ebp));
    
    /* Get pointer to kernel stack */
    t->kernel_stack = kmalloc(KERNEL_STACK_SIZE);
    
    /* Prepare stack to grow DOWN in memory. */
    t->context.esp = (uint32_t)t->kernel_stack + KERNEL_STACK_SIZE;
    t->context.ebp = t->context.esp;
    
    /*
     * Point the thread at our forced starting point.
     * This makes it easy to tell when a thread has finished
     * execution so we can do the clean-up work
     * seamlessly.
     */
    t->context.eip = (uint32_t)thread_start_execution;
    
    /* Point at the user's requested function. */
    t->start_func = func;
    t->finished = 0;
    
    /* The thread is now ready to execute. */
    make_thread_ready(t);
}

/*
 * Although it should be noted that thread yielding is
 * unnecessary with preemption (which has been implemented),
 * it is a fun feature when playing with multithreading
 * facilities.
 */
void thread_yield()
{
    assert(current_thread);
    task_switch();
}

/*
 * Yields execution of the thread. Does not requeue for
 * further execution (this must be triggered by an external
 * thread).
 */
void thread_sleep()
{
    assert(current_thread);
    current_thread->sleeping = 1;
    thread_yield();
}
