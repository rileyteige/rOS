#include <system.h>
#include <video.h>

void kmain()
{
	extern uint32_t magic;

	if (magic != MULTIBOOT_MAGIC)
		return;

	cli();    

    gdt_init();
    //idt_init();

	clear();
	int i = 0;
	for (i = 0; i <= 100; i += 5)
	{
	    put_int(i);
	    put_char('\n');
	}
}
