#include <system.h>
#include <video.h>

#define TIMER_IRQ 0
#define TIMER_FREQUENCY 200

/* Programmable Interrupt Timer (PIT) definitions */
#define PIT 0x40
#define PIT_CONTROL 0x43

#define PIT_MASK 0xFF
#define PIT_FREQ 1193182
#define PIT_SET_COMMAND 0x36

uint32_t tick = 0;
uint32_t time = 0;

/*
 * Set the frequency of the PIT in hertz.
 */
void timer_set_frequency(int hz)
{
    int divisor = PIT_FREQ / hz;
    outportb(PIT_CONTROL, PIT_SET_COMMAND);
    outportb(PIT, divisor & PIT_MASK);
    outportb(PIT, (divisor >> 8) & PIT_MASK);
}

/*
 * Timer IRQ handler
 */
void timer_interrupt_handler(struct regs* r)
{
    tick++;
    if (tick == TIMER_FREQUENCY * 10) {
        time++;
        tick = 0;
    }
    irq_finish();
    task_switch();
}

/*
 * Timer initializer
 */
void timer_init()
{
    irq_register_handler(TIMER_IRQ, timer_interrupt_handler);
    timer_set_frequency(TIMER_FREQUENCY);
}
