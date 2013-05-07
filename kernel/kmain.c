#include <multiboot.h>
#include <system.h>
#include <video.h>

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
    
    /* Kernel heap */
    heap_init();
    
    /* Drivers */
    timer_init();
    tasking_init();
    keyboard_init();
    
    /* Let the games begin */
    sti();
    
    test_kmalloc_kfree();
    
    for (;;);
}
