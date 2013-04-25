#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <grub.h>
#include <types.h>

extern int max(int a, int b);
extern int min(int a, int b);
extern int abs(int a);

extern void* memcpy(void* dest, const void* src, size_t count);
extern void* memset(void* loc, int val, size_t size);

extern unsigned int strlen(const char *str);

/* Descriptor Tables */
extern void gdt_init();

extern void idt_init();
extern void idt_set_entry(uint8_t idx, uint32_t base, uint16_t selector, uint8_t flags);

extern void irq_init();
extern void irq_register_handler(int irq, irq_handler_t handler);
extern void irq_unregister_handler(int irq);

/* Devices */
extern void timer_init();
extern void timer_set_frequency(int hz);

/* Ports */
extern uint8_t inportb(uint16_t port);
extern void outportb(uint16_t port, uint8_t data);

/* Interrupts Flag */
extern void cli();
extern void sti();

#define NULL ((void*)0)

#endif // __SYSTEM_H
