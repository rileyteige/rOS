#ifndef __CONTEXT_H
#define __CONTEXT_H

#include <types.h>

typedef struct {
    uint32_t ebx, edx;
    uint32_t eip, esp, edi, esi, ebp;
    void* kernel_stack;
} __attribute__((packed)) context_t;

extern void context_save(context_t* c);

#endif // __CONTEXT_H
