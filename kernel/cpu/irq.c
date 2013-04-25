#include <system.h>

#define NUM_IRQ_ROUTINES 16

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

void_fn irq_functions[NUM_IRQ_ROUTINES] = {
    _irq0,
    _irq1,
    _irq2,
    _irq3,
    _irq4,
    _irq5,
    _irq6,
    _irq7,
    _irq8,
    _irq9,
    _irq10,
    _irq11,
    _irq12,
    _irq13,
    _irq14,
    _irq15
};

irq_handler_t irq_routines[NUM_IRQ_ROUTINES] = { NULL };

void irq_register_handler(int irq, irq_handler_t handler)
{
    irq_routines[irq] = handler;
}

void irq_unregister_handler(int irq)
{
    irq_routines[irq] = 0;
}

#define CODE_SELECTOR 0x08
#define IRQ_FLAGS 0x8E

void register_irq_entries()
{
    int offset = 32;
    int i = 0;
    for (i = 0; i < NUM_IRQ_ROUTINES; i++)
        idt_set_entry(i + offset, (unsigned)irq_functions[i], CODE_SELECTOR, IRQ_FLAGS);
}

/* PIC remapping courtesy of OSDev.org */

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1_COMMAND + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2_COMMAND + 1)

#define ICW1_ICW4 0x01       /* ICW4 (not) needed */
#define ICW1_SINGLE 0x02     /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04  /* Call address interval 4 (8) */
#define ICW1_LEVEL 0x08      /* Level triggered (edge) mode */
#define ICW1_INIT 0x10       /* Initialization - required! */
 
#define ICW4_8086 0x01       /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02       /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE 0x08  /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM 0x10       /* Special fully nested (not) */

void pic_remap()
{
    uint8_t mask1, mask2;
    
    mask1 = inportb(PIC1_DATA);
    mask2 = inportb(PIC2_DATA);

    outportb(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);
    outportb(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);
    
    outportb(PIC1_DATA, 0x20);
    outportb(PIC2_DATA, 0x28);
    
    outportb(PIC1_DATA, 0x04);
    outportb(PIC2_DATA, 0x02);
    
    outportb(PIC1_DATA, ICW4_8086);
    outportb(PIC2_DATA, ICW4_8086);
    
    outportb(PIC1_DATA, mask1);
    outportb(PIC2_DATA, mask2);
}

void irq_finish()
{
    outportb(0x20, 0x20);
}

void irq_init()
{
    pic_remap();
    register_irq_entries();
}

void irq_router(struct regs* r)
{
    int int_num = r->int_num;
    
    cli();
    
    irq_handler_t handler;
    
    if (int_num < 32 || int_num > 47) {
        handler = NULL;
    } else {
        handler = irq_routines[int_num - 32];
    }
    
    if (handler != NULL)
        handler(r);
    
    irq_finish();
    
    sti();
}
