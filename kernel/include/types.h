#ifndef __TYPES_H
#define __TYPES_H

typedef unsigned long size_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

struct regs {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_num, error_code;
    uint32_t eip, cs, eflags, user_esp, ss;
} __attribute__((packed));

typedef void (*void_fn) ();
typedef void (*irq_handler_t) (struct regs*);

#endif // __TYPES_H
