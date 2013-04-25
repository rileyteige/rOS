#include <system.h>
#include <video.h>

#define TIMER_IRQ 0

void timer_interrupt_handler(struct regs* r)
{
    r = r;
    put_char('a');
}

void timer_init()
{
    irq_register_handler(TIMER_IRQ, timer_interrupt_handler);
}
