#include <multiboot.h>
#include <system.h>
#include <types.h>
#include <video.h>

#define MEMORY_AVAILABLE 1
#define MMAP_SIZE_THRESHOLD 1024

uint8_t memory_initialized = 0;
memory_map_t kernel_mem;

void dump_memory_map(memory_map_t* mmap)
{
    kprintf("%d %d %d %d %s\n",
        mmap->base_addr_low,
        mmap->length_low / 1024,
        mmap->base_addr_high,
        mmap->length_high / 1024,
        mmap->type == MEMORY_AVAILABLE ? "Available" : "Unavailable");
}

void process_memory_map(multiboot_info_t* mbt)
{
    memory_initialized = 1;
    memory_map_t* mmap = (memory_map_t*)mbt->mmap_addr;
    while ((uint32_t)mmap < (uint32_t)mbt->mmap_addr + mbt->mmap_length)
    {
        dump_memory_map(mmap);
        
        if (mmap->type == MEMORY_AVAILABLE && mmap->length_low / 1024 > MMAP_SIZE_THRESHOLD)
            kernel_mem = *mmap;
            
        mmap = (memory_map_t*) ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
    }
}

void* kmalloc(size_t bytes)
{
    return NULL;
}

void kfree(void* ptr)
{
    assert(ptr != NULL);
    assert(memory_initialized);
}
