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

void dump_stack(uint32_t loc, uint32_t how_far_back)
{
    uint32_t low_loc = loc - how_far_back;
    
    assert(!(how_far_back % 16));
    
    uint16_t* mem = (uint16_t*)(loc - 8);
    while ((uint32_t)mem >= low_loc)
    {
        const int DW_PER_ROW = 8;
        
        uint16_t* start = (uint16_t*)((uint32_t)mem - DW_PER_ROW * 2);
        kprintf("  0x%8x: ", start);
        
        int i = 0;
        for (i = 0; i < DW_PER_ROW; i++)
        {
            uint16_t* val = (uint16_t*)((uint32_t)start + i * 2);
            kprintf("0x%4x ", *val);
        }
        kprintf("\n");
        
        mem = (uint16_t*)((uint32_t)mem - DW_PER_ROW * 2);
    }
}

void unhandled_exception(uint32_t interrupt, struct regs* r)
{
    if (interrupt == DOUBLE_FAULT)
        halt_execution();

    assert(interrupt < 32);
    
    const char* border = "-------------------------------------------------------------------------\n";
    
    reset();
    kprintf(border);
    kprintf("  UNHANDLED KERNEL EXCEPTION: %s\n", exception_names[interrupt]);
    kprintf(border);
    kprintf("  eax: 0x%8x   ebx: 0x%8x   ecx: 0x%8x   edx: 0x%8x\n", r->eax, r->ebx, r->ecx, r->edx);
    kprintf("  edi: 0x%8x   esi: 0x%8x   ebp: 0x%8x   esp: 0x%8x\n", r->edi, r->esi, r->ebp, r->esp);
    kprintf("   gs: 0x%8x    fs: 0x%8x    es: 0x%8x    ds: 0x%8x\n", r->gs, r->fs, r->es, r->ds);
    kprintf("  eip: 0x%8x    cs: 0x%8x  eflg: 0x%8x    ss: 0x%8x\n", r->eip, r->cs, r->eflags, r->ss);
    kprintf("\n\n");
    dump_stack(r->esp, 0x80);
    kprintf(border);
    
    halt_execution();
}
