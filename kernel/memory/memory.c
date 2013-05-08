#include <multiboot.h>
#include <system.h>
#include <types.h>
#include <video.h>

#define HEAP_ENTRY_USED 1
#define HEAP_ENTRY_CHECK 42

uint8_t memory_initialized = 0;

typedef struct heap_entry {
    struct heap_entry* prev;
    struct heap_entry* next;
    size_t size;
    uint8_t check;
    uint8_t flags;
    void* data;
} heap_entry_t;

typedef struct {
    heap_entry_t* first;
    size_t size_available;
} __attribute__((packed)) heap_t;

static uint8_t heap_arr[HEAP_SIZE];
static heap_t heap = { (heap_entry_t*)heap_arr, HEAP_SIZE };

/*
 * Aligns a number upward to the nearest value evenly
 * divisible by "align_to".
 */
static inline size_t align(unsigned int i, unsigned int align_to)
{
    return i % align_to ? i + (align_to - i % align_to) : i;
}

/*
 * Heap initializer
 */
void heap_init()
{
    kprintf("Initializing kernel heap...\n");

    size_t heap_header_size = align(sizeof(heap_entry_t) - sizeof(void*), 4);
    
    heap.first->prev = NULL;
    heap.first->next = NULL;
    heap.first->size = HEAP_SIZE - heap_header_size;
    heap.first->flags = 0;
    heap.first->check = HEAP_ENTRY_CHECK;
    
    /* Space for the first header */
    heap.size_available -= sizeof(heap_entry_t) - sizeof(void*);
    
    memory_initialized = 1;
}

size_t entry_size(heap_entry_t* entry)
{
    return align(sizeof(heap_entry_t) - sizeof(void*), 4) + entry->size;
}

/*
 * Splits a heap entry into two blocks, one equal to the
 * requested size, the other representing the space remaining
 * after removing the requested size as well as making room
 * for bookkeeping.
 */
void split_heap_entry(heap_entry_t* old_entry, size_t size)
{
    assert(old_entry != NULL);
    
    uint32_t new_entry_addr = align((int)&old_entry->data, 4) + size;
    heap_entry_t* new_entry = (heap_entry_t*)new_entry_addr;
    
    assert(new_entry != NULL);
    
    new_entry->prev = old_entry;
    new_entry->next = old_entry->next;
    old_entry->next = new_entry;

    if (new_entry->next)
        new_entry->next->prev = new_entry;
    
    new_entry->check = HEAP_ENTRY_CHECK;    
    new_entry->flags = 0;
    
    new_entry->size = old_entry->size - size - align(sizeof(heap_entry_t) - sizeof(void*), 4);
    
    old_entry->size = size;
}

/*
 * Joins one heap entry into the next.
 */
void join_heap_entry(heap_entry_t* entry)
{
    assert(entry->next != NULL);
    /*
     * Simply increase the size of the entry to encapsulate
     * the next entry; no need to alter the data inside that
     * block. Then point after the block and make anything following
     * it point back at the encapsulating entry.
     */
    entry->size += entry->next->size + align(sizeof(heap_entry_t) - sizeof(void*), 4);
    entry->next = entry->next->next;
    if (entry->next)
        entry->next->prev = entry;
}

/*
 * Implements first-fit dynamic memory allocation
 */
void* kmalloc(size_t bytes)
{
    heap_entry_t* entry = heap.first;
    uint32_t addr = -1;
    
    assert(bytes <= heap.size_available);
    
    bytes = align(bytes, 4);
    
    while (entry && (entry->size < bytes || entry->flags & HEAP_ENTRY_USED))
        entry = entry->next;
    
    if (entry == NULL)
        return NULL;
    
    if (entry->size > bytes)
        split_heap_entry(entry, bytes);
    
    entry->flags |= HEAP_ENTRY_USED;

    heap.size_available -= entry_size(entry);
    
    addr = align((int)&entry->data, 4);
    
    return (void*)addr;
}

/*
 * Marks a heap entry as unused, joining together unused neighbors.
 */
void kfree(void* ptr)
{
    heap_entry_t* entry = NULL;
    
    assert(ptr != NULL);
    assert(memory_initialized);
    
    entry = ptr - align(sizeof(heap_entry_t) - sizeof(void*), 4);
    
    assert(entry->check == HEAP_ENTRY_CHECK);
    assert(entry->flags & HEAP_ENTRY_USED);
    
    entry->flags ^= HEAP_ENTRY_USED;
    heap.size_available += entry_size(entry);
    
    assert(heap.size_available <= HEAP_SIZE);
    
    if (entry->prev && !(entry->prev->flags & HEAP_ENTRY_USED)) {
        entry = entry->prev;
        join_heap_entry(entry->prev);
    }
    
    if (entry->next && !(entry->next->flags & HEAP_ENTRY_USED))
        join_heap_entry(entry);
}
