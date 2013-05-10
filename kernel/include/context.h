#ifndef __CONTEXT_H
#define __CONTEXT_H

#include <types.h>

typedef struct {
    uint32_t eip, esp, ebp;
    int id, finished;
    void* kernel_stack;
} __attribute__((packed)) context_t;

#endif // __CONTEXT_H
