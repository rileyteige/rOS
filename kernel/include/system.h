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

/* Interrupts Flag */
extern void cli();
extern void sti();

#endif // __SYSTEM_H
