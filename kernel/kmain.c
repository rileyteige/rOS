#include <multiboot.h>
#include <system.h>
#include <video.h>

void kmain(multiboot_info_t* mbt, unsigned int magic)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
        return;

    cli();
    reset();
    
    process_memory_map(mbt);

    /* Core modules */
    gdt_init();
    idt_init();
    irq_init();
    
    /* Device drivers */
    timer_init();
    keyboard_init();
    
    /* Let the games begin */
    sti();
    
    for (;;);
}
