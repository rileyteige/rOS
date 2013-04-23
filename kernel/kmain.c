#include <stdint.h>

#include <system.h>
#include <video.h>

void kmain()
{
	extern uint32_t magic;

	if (magic != 0x2BADB002)
		return;

	set_interrupt_status(DISABLED);

	clear();
	int i = 0;
	for (i = 0; i <= 100; i += 5)
	{
	    put_int(i);
	    put_char('\n');
	}
	
	set_interrupt_status(ENABLED);
}
