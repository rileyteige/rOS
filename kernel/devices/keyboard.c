#include <system.h>
#include <video.h>

#define KEYBOARD_IRQ 1

#define KEY_SIGNAL 0x60
#define KEY_PENDING 0x64

void wait()
{
    while (inportb(KEY_PENDING) & 0x02);
}

uint8_t accept_key()
{
    wait();
    return inportb(KEY_SIGNAL);
}

void keyboard_interrupt_handler(struct regs* r)
{
    accept_key();
    irq_finish();
}

void keyboard_init()
{
    irq_register_handler(KEYBOARD_IRQ, keyboard_interrupt_handler);
}
