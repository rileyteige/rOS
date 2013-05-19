#include <list.h>
#include <multiboot.h>
#include <system.h>
#include <thread.h>
#include <video.h>

#define testing() kprintf("Testing %s...\n", test_name)
#define test_failed(x) { kprintf("%s test failed: %s\n", test_name, x); return; }

void test_list()
{
    int actuals[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    const char* test_name = "linked list";
    
    testing();
    
    int i = 0;
    list_t* list = list_create();
    
    if (!list)
        test_failed("null list");
    
    for (i = 0; i < 10; i++) {
        int* val = kmalloc(sizeof(int));
        *val = i + 1;
        list_push(list, val);
    }
    
    int* val = NULL;
    for (i = 9; i >= 0; i--) {
        val = (int*)list_pop(list);
        if (!val)
            test_failed("null value");
        if (*val != actuals[i]) {
            kprintf("*val == %d, actuals[%d] == %d\n", *val, i, actuals[i]);
            test_failed("bad value");
        }
    }
    
    list_destroy(list);
}

void test_kmalloc_kfree()
{
    int actuals[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    const char* test_name = "heap";
    
    testing();
    
    int i = 0;
    int* nums = kmalloc(10 * sizeof(int));
    if (!nums)
        test_failed("null kmalloc");
    
    for (i = 0; i < 10; i++) {
        nums[i] = i + 1;
    }
    
    for(i = 0; i < 10; i++) {
        if (nums[i] != actuals[i])
            test_failed("bad value");
    }
    
    kfree(nums);
}

#define NUM_THREADS 10
int finished[NUM_THREADS] = { 0 };
int x = 0;
mutex_t* test_mutex = NULL;

void test_thread_start()
{
    thread_t* t = get_current_thread();
    assert(t);
    int i, j, k;
    i = j = k = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 5; j++) {
            mutex_lock(test_mutex);
            for (k = 0; k < 500; k++) {
                x++;
                x--;
            }
            mutex_unlock(test_mutex);
        }
        for (j = 0; j < t->id; j++) {
            thread_yield();
        }
    }

    finished[t->id - 2] = 1;
}

void test_tasking()
{
    const char* test_name = "tasking";

    kprintf("Testing %s...", test_name);

    test_mutex = mutex_create();
    
    int j = 0;
    for (j = 0; j < 10; j++) {
        x = 0;
        thread_t* t[NUM_THREADS] = { NULL };
        
        int i = 0;
        for (i = 0; i < NUM_THREADS; i++) {
            t[i] = thread_create(i + 2);
            finished[i] = 0;
        }
        
        for (i = 0; i < NUM_THREADS; i++)
            thread_start(t[i], test_thread_start);

        int done = 0;
        while (!done) {
            done = 1;
            for (i = 0; i < NUM_THREADS; i++) {
                if (!finished[i]) {
                    done = 0;
                    break;
                }  
            }
            thread_yield();
        }

        kprintf(".");
        if (x != 0) {
            kprintf("\n");
            test_failed("x != 0");
        }
    }
    kprintf("\n");
    
    mutex_free(test_mutex);
}

void kmain(multiboot_info_t* mbt, unsigned int magic)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
        return;

    graphics_init();

    /* Core modules */
    gdt_init();
    idt_init();
    irq_init();
    isr_init();
    
    /* Kernel heap */
    heap_init();
    
    /* Drivers */
    timer_init();
    tasking_init();
    keyboard_init();
    
    /* Let the games begin */
    set_interrupts(ENABLED);
    
    kprintf("Running kernel tests...\n\n");
    test_kmalloc_kfree();
    test_list();
    test_tasking();
    kprintf("\nDone with kernel tests.\n");
    
    for (;;);
}
