#include <list.h>
#include <system.h>
#include <thread.h>

#define THREAD_MAGIC 0xDEADBEEF

list_t* ready_queue;
list_t* kill_queue;
thread_t* current_thread;

void switch_next_task();

/*
 * Multi-tasking initializer.
 */
void tasking_init()
{
    ready_queue = list_create();
    kill_queue = list_create();
    current_thread = thread_create(0);
}

/*
 * Returns a pointer to the current thread
 * of execution.
 */
thread_t* get_current_thread()
{
    return current_thread;
}

/*
 * Oversees a context switch between two executing
 * thread.
 */
void task_switch()
{
    cli();
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
        list_empty(kill_queue);
        sti();
        return;
    }
    
    current_thread->context.eip = eip;
    current_thread->context.esp = esp;
    current_thread->context.ebp = ebp;
    
    if (!current_thread->finished)
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
