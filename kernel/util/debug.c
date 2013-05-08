#include <system.h>
#include <video.h>

void panic(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    reset();
    kprintf("----- KERNEL PANIC -----\n\n");
    kvprintf(format, args);
    halt_execution();
    va_end(args);
}

#define DOUBLE_FAULT 8

const char* exception_names[] = {
    "DIVIDE-BY-ZERO",
    "DEBUG",
    "NON-MASKABLE INTERRUPT",
    "BREAKPOINT",
    "OVERFLOW",
    "BOUND RANGE EXCEEDED",
    "INVALID OPCODE",
    "DEVICE NOT AVAILABLE",
    "DOUBLE FAULT",
    "COPROCESSOR SEGMENT OVERRUN",
    "INVALID TSS",
    "SEGMENT NOT PRESENT",
    "STACK-SEGMENT FAULT",
    "GENERAL PROTECTION FAULT",
    "PAGE FAULT",
    "RESERVED",
    "x87 FLOATING-POINT EXCEPTION",
    "ALIGNMENT CHECK",
    "MACHINE CHECK",
    "SIMD FLOATING-POINT EXCEPTION",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "SECURITY EXCEPTION",
    "RESERVED",
    "TRIPLE FAULT"
};

void unhandled_exception(uint32_t interrupt, struct regs* r)
{
    if (interrupt == DOUBLE_FAULT)
        halt_execution();

    assert(interrupt < 32);
    
    panic("EXCEPTION: %s", exception_names[interrupt]);
}
