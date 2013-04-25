#include <system.h>
#include <video.h>

void kmain()
{
    extern uint32_t magic;

    if (magic != MULTIBOOT_MAGIC)
        return;

    cli();    

    clear();

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
