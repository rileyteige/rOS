#include <system.h>
#include <types.h>

extern void gdt_refresh();

struct gdt_desc {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

/* GDT Pointer */
struct gdt_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#define DEFAULT_GRANULARITY 0xCF
#define DEFAULT_BASE 0
#define DEFAULT_LIMIT 0xFFFFFFFF

#define NUM_GDT_ENTRIES 6

struct gdt_desc gdt[NUM_GDT_ENTRIES];
struct gdt_t gdt_ptr;

void gdt_set_entry(int idx, size_t base, size_t limit, uint8_t access, uint8_t granularity)
{
    gdt[idx].base_low = (base & 0xFFFF);
    gdt[idx].base_middle = (base >> 16) & 0xFF;
    gdt[idx].base_high = (base >> 24) & 0xFF;
    
    gdt[idx].limit_low = (limit & 0xFFFF);
    gdt[idx].granularity = (limit >> 16) & 0x0F;
    gdt[idx].granularity |= (granularity & 0xF0);
    
    gdt[idx].access = access;
}

void gdt_init()
{
    gdt_ptr.limit = sizeof(struct gdt_desc) * NUM_GDT_ENTRIES - 1;
    gdt_ptr.base = (uint32_t)&gdt;
    
    /* Null Entry */
    gdt_set_entry(0, 0, 0, 0, 0);
    /* Code Segment */
    gdt_set_entry(1, DEFAULT_BASE, DEFAULT_LIMIT, 0x9A, DEFAULT_GRANULARITY);
    /* Data Segment */
    gdt_set_entry(2, DEFAULT_BASE, DEFAULT_LIMIT, 0x92, DEFAULT_GRANULARITY);

    gdt_refresh();
}
