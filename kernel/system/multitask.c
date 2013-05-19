#include <list.h>
#include <system.h>
#include <thread.h>

#define THREAD_MAGIC 0xDEADBEEF

list_t* ready_queue = NULL;
list_t* kill_queue = NULL;
thread_t* current_thread = NULL;

void switch_next_task();

void spin()
{
    for (;;)
        thread_yield();
}

/*
 * Multi-tasking initializer.
 */
void tasking_init()
{
    ready_queue = list_create();
    kill_queue = list_create();
    current_thread = thread_create(1);
    thread_t* idle_thread = thread_create(0);
    thread_start(idle_thread, spin);
}

/*
 * Returns a pointer to the current thread
 * of execution.
 */
thread_t* get_current_thread()
{
    return current_thread;
}

uint8_t is_multitasking_running()
{
    return current_thread != NULL;
}

void clean_up_threads()
{
    list_empty(kill_queue);
}

void make_thread_ready(thread_t* t)
{
    assert(t);
    assert(!t->finished);

    t->sleeping = 0;

    uint8_t old_status = set_interrupts(DISABLED);

    list_enqueue(ready_queue, t);

    set_interrupts(old_status);
}

/*
 * Oversees a context switch between two executing
 * thread.
 */
void task_switch()
{
    set_interrupts(DISABLED);

    if (!is_multitasking_running())
        return;

    assert(current_thread);

    uint32_t esp, ebp, eip;
    asm volatile ("mov %%esp, %0" : "=r" (esp));
    asm volatile ("mov %%ebp, %0" : "=r" (ebp));
    
    /*
     * If we make the eip register hold the value
     * returned by this function, execution will
     * resume just as this function returns. In
     * our case, we jump back to this point, but
     * having forced eax to hold THREAD_MAGIC
     * as opposed to the actual eip value, meaning
     * the context switch is complete.
     */
    eip = read_pc();
    if (eip == THREAD_MAGIC) {
        clean_up_threads();
        set_interrupts(ENABLED);
        return;
    }
    
    current_thread->context.eip = eip;
    current_thread->context.esp = esp;
    current_thread->context.ebp = ebp;
    
    if (!(current_thread->finished || current_thread->sleeping))
        list_enqueue(ready_queue, current_thread);

    switch_next_task();
}

/*
 * Performs a context switch to the next
 * ready thread.
 */
void switch_next_task()
{
    uint32_t esp, ebp, eip;
    current_thread = (thread_t*)list_dequeue(ready_queue);
    
    assert(current_thread);
    
    eip = current_thread->context.eip;
    esp = current_thread->context.esp;
    ebp = current_thread->context.ebp;
    
    /* Make the jump */
    asm volatile(
        "mov %0, %%ebx\n"
        "mov %1, %%esp\n"
        "mov %2, %%ebp\n"
        "mov %3, %%eax\n"
        "jmp *%%ebx"
        : : "r" (eip), "r" (esp), "r" (ebp), "r" (THREAD_MAGIC)
        : "%ebx", "%esp", "%eax"
    );
}
