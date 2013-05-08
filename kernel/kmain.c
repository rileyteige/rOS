#include <list.h>
#include <multiboot.h>
#include <system.h>
#include <video.h>

void test_list()
{
    kprintf("Testing list...\n");
    int i = 0;
    list_t* list = list_create();
    for (i = 0; i < 10; i++) {
        int* val = kmalloc(sizeof(int));
        *val = i + 1;
        list_push(list, val);
    }
    
    kprintf("Nums: ");
    int* val;
    while ((val = (int*)list_pop(list))) {
        kprintf("%d ", *val);
    }
    kprintf("\n");
    
    list_destroy(list);
    
    kprintf("List test passed.\n");
}

void test_kmalloc_kfree()
{
    kprintf("Testing kmalloc...\n");
    int i = 0;
    int* nums = kmalloc(10 * sizeof(int));
    if (nums == NULL) {
        kprintf("kmalloc test failed.\n");
        return;
    }
    for (i = 0; i < 10; i++) {
        nums[i] = i + 1;
    }
    kprintf("Nums: ");
    for(i = 0; i < 10; i++)
        kprintf("%d ", nums[i]);
    kprintf("\n");
    kprintf("Calling kfree...\n");
    kfree(nums);
    kprintf("kmalloc test passed.\n");
}

void kmain(multiboot_info_t* mbt, unsigned int magic)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
        return;

    cli();
    reset();

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
    
    test_kmalloc_kfree();
    test_list();
    
    for (;;);
}
