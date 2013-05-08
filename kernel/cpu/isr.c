#include <system.h>

#define NUM_ISR_ROUTINES 32

extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

void_fn isr_functions[NUM_ISR_ROUTINES] = {
    _isr0,
    _isr1,
    _isr2,
    _isr3,
    _isr4,
    _isr5,
    _isr6,
    _isr7,
    _isr8,
    _isr9,
    _isr10,
    _isr11,
    _isr12,
    _isr13,
    _isr14,
    _isr15,
    _isr16,
    _isr17,
    _isr18,
    _isr19,
    _isr20,
    _isr21,
    _isr22,
    _isr23,
    _isr24,
    _isr25,
    _isr26,
    _isr27,
    _isr28,
    _isr29,
    _isr30,
    _isr31
};

irq_handler_t isr_routines[256] = { NULL };

void isr_register_handler(int isr, irq_handler_t handler)
{
    isr_routines[isr] = handler;
}

void isr_unregister_handler(int isr)
{
    isr_routines[isr] = NULL;
}

#define CODE_SELECTOR 0x08
#define IRQ_FLAGS 0x8E

void isr_init()
{
    int i = 0;
    for (i = 0; i < NUM_ISR_ROUTINES; i++) {
        idt_set_entry(i, (unsigned)isr_functions[i], CODE_SELECTOR, IRQ_FLAGS);
    }
}

void isr_router(struct regs *r)
{
    uint32_t int_num = r->int_num;
    unhandled_exception(int_num, r);
}
