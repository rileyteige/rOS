#include <system.h>
#include <video.h>

void kmain()
{
    extern uint32_t magic;

    if (magic != MULTIBOOT_MAGIC)
        return;

    cli();    

    clear();

    gdt_init();
    idt_init();
    irq_init();
    
    sti();
    
    for (;;);
}
