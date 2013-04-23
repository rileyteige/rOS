#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdint.h>
#include <types.h>

extern int max(int a, int b);
extern int min(int a, int b);
extern int abs(int a);

extern void* memcpy(void* dest, const void* src, size_t count);

extern unsigned int strlen(const char *str);

extern interrupt_status set_interrupt_status(interrupt_status status);

#endif // __SYSTEM_H
