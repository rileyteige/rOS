#include <list.h>
#include <system.h>
#include <thread.h>

#define THREAD_MAGIC 0xDEADBEEF

list_t* ready_queue;
list_t* kill_queue;
thread_t* current_thread;

void switch_next_task();

int finished[3] = { 0 };

void test_thread_start()
{
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 1000000; j++);
        kprintf("Hello from thread %d\n", current_thread->id);
        for (j = 0; j < 50 * current_thread->id; j++)
            task_switch();
    }
    finished[current_thread->id - 1] = 1;
    thread_finish(current_thread);
}

void tasking_init()
{
    ready_queue = list_create();
    kill_queue = list_create();
    current_thread = thread_create(0);
}

void _test_tasking()
{
    thread_t* test = thread_create(1);
    thread_t* test2 = thread_create(2);
    thread_t* test3 = thread_create(3);
    thread_t* test4 = thread_create(4);
    
    thread_start(test, test_thread_start);
    thread_start(test2, test_thread_start);
    thread_start(test3, test_thread_start);
    thread_start(test4, test_thread_start);
    
    while (!(finished[0] && finished[1] && finished[2] && finished[3]));
}

void task_switch()
{
    cli();
    uint32_t esp, ebp, eip;
    asm volatile ("mov %%esp, %0" : "=r" (esp));
    asm volatile ("mov %%ebp, %0" : "=r" (ebp));
    eip = read_pc();
    if (eip == THREAD_MAGIC) {
        /*
         * This is where we jump back to
         * in switch_next_task()
         */
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
