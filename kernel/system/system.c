// Author: Riley Teige

#include <system.h>

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

int abs(int a)
{
    return a < 0 ? -a : a;
}

void* memcpy(void* dest, const void* src, size_t count)
{
    size_t i = 0;
    for (i = 0; i < count; i++) {
        ((unsigned char*)dest)[i] = ((unsigned char*)src)[i];
    }
    
    return dest;
}

void* memset(void* loc, int val, size_t size)
{
    size_t i = 0;
    for (i = 0; i < size; i++)
        ((int*)loc)[i] = val;
    
    return loc;
}

uint8_t inportb(uint16_t port)
{
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void outportb(uint16_t port, uint8_t data)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

uint16_t inports(uint16_t port)
{
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void outports(uint16_t port, uint16_t data)
{
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (data));
}

unsigned int strlen(const char *str)
{
    int i = 0;
    while (str[i] != (char)0)
        ++i;
    return i;
}

void cli()
{
    asm("cli");
}

void sti()
{
    asm("sti");
}
