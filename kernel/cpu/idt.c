#include <system.h>

extern void idt_refresh();

/* IDT Entry */
struct idt_entry_t {
    uint16_t offset_1; /* Offset bits 0..15 */
    uint16_t selector; /* A code selector */
    uint8_t zero;      /* Should always be zero */
    uint8_t type_attr; /* Type/Attributes */
    uint16_t offset_2; /* Offset bits 16..31 */
};

/* IDT */
struct idt_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#define NUM_IDT_ENTRIES 256

struct idt_entry_t idt[NUM_IDT_ENTRIES];
struct idt_t idt_ptr;

#define INTERRUPT_FLAG 0x60

void idt_set_entry(uint8_t idx, uint32_t base, uint16_t selector, uint8_t flags)
{
    idt[idx].offset_1 = base & 0xFFFF;
    idt[idx].offset_2 = (base >> 16) & 0xFFFF;
    idt[idx].selector = selector;
    idt[idx].zero = 0;
    idt[idx].type_attr = flags | INTERRUPT_FLAG;
}

void idt_init()
{
    idt_ptr.limit = sizeof(struct idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt;
    memset(&idt, 0, sizeof(struct idt_entry_t) * 256);
    
    idt_refresh();
}
