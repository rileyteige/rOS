// Author: Riley Teige

#include <system.h>

static interrupt_status current_interrupt_status = DISABLED;

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

unsigned int strlen(const char *str)
{
    int i = 0;
    while (str[i] != (char)0)
        ++i;
    return i;
}

void clear_interrupts()
{
    asm("cli");
}

void set_interrupts()
{
    asm("sti");
}

interrupt_status set_interrupt_status(interrupt_status status)
{
    interrupt_status old_status = current_interrupt_status;
    
    if (status == DISABLED) {
        clear_interrupts();
    } else {
        set_interrupts();
    }
    
    current_interrupt_status = status;
    return old_status;
}
