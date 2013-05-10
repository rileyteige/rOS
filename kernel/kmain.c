#include <list.h>
#include <multiboot.h>
#include <system.h>
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

extern void _test_tasking();

void test_tasking()
{
    const char* test_name = "tasking";

    testing();
    _test_tasking();
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
    sti();
    
    kprintf("Running kernel tests...\n\n");
    test_kmalloc_kfree();
    test_list();
    test_tasking();
    kprintf("\nDone with kernel tests.\n");
    
    for (;;);
}
