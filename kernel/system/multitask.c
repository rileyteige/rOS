#include <context.h>
#include <list.h>
#include <system.h>
#include <video.h>

#define THREAD_MAGIC 0xDEADBEEF

list_t* ready_queue;
list_t* kill_queue;
context_t* current_context;

void switch_next_task();
void thread_finish();

int finished[3] = { 0 };

void test_thread_start()
{
    int i, j;
    for (i = 0; i < 5; i++) {
        kprintf("Hello from thread %d\n", current_context->id);
        for (j = 0; j < current_context->id * 50; j++)
            task_switch();
    }
    thread_finish();
}

void tasking_init()
{
    ready_queue = list_create();
    kill_queue = list_create();
    current_context = (context_t*)kmalloc(sizeof(context_t));
    
    current_context->id = 1;
    current_context->finished = 0;
}

void thread_finish()
{
    finished[current_context->id - 2] = 1;
    current_context->finished = 1;
    list_enqueue(kill_queue, current_context);
    task_switch();
}

void _test_tasking()
{
    context_t* test = (context_t*)kmalloc(sizeof(context_t));
    context_t* test2 = (context_t*)kmalloc(sizeof(context_t));
    context_t* test3 = (context_t*)kmalloc(sizeof(context_t));

    test->id = 2;
    test2->id = 3;
    test3->id = 4;
    
    uint32_t esp, ebp;
    asm volatile ("mov %%esp, %0" : "=r" (esp));
    asm volatile ("mov %%ebp, %0" : "=r" (ebp));
    test->esp = esp - 0x1000;
    test->ebp = ebp - 0x1000;
    test2->esp = esp - 0x2000;
    test2->ebp = ebp - 0x2000;
    test3->esp = esp - 0x3000;
    test3->ebp = ebp = 0x3000;
    
    test->finished = 0;
    test2->finished = 0;
    test3->finished = 0;
    
    test->eip = (uint32_t)test_thread_start;
    test2->eip = (uint32_t)test_thread_start;
    test3->eip = (uint32_t)test_thread_start;
    list_enqueue(ready_queue, test);
    list_enqueue(ready_queue, test2);
    list_enqueue(ready_queue, test3);
    
   while (!(finished[0] && finished[1] && finished[2]));
}

void task_switch()
{        
    uint32_t esp, ebp, eip;
    asm volatile ("mov %%esp, %0" : "=r" (esp));
    asm volatile ("mov %%ebp, %0" : "=r" (ebp));
    eip = read_pc();
    if (eip == THREAD_MAGIC) {
        list_empty(kill_queue);
        return;
    }
    
    current_context->eip = eip;
    current_context->esp = esp;
    current_context->ebp = ebp;
    
    if (!current_context->finished)
        list_enqueue(ready_queue, current_context);

    switch_next_task();
}

void switch_next_task()
{
    uint32_t esp, ebp, eip;
    current_context = (context_t*)list_dequeue(ready_queue);
    eip = current_context->eip;
    esp = current_context->esp;
    ebp = current_context->ebp;
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
