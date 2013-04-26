#include <multiboot.h>
#include <system.h>
#include <types.h>
#include <video.h>

void dump_memory_map(memory_map_t* mmap)
{
    kprintf("%d %d %d %d %s\n", mmap->base_addr_low, mmap->length_low / 1024, mmap->base_addr_high, mmap->length_high / 1024, mmap->type == 1 ? "Available" : "Unavailable");
}

void process_memory_map(multiboot_info_t* mbt)
{
    memory_map_t* mmap = (memory_map_t*)mbt->mmap_addr;
    while ((uint32_t)mmap < (uint32_t)mbt->mmap_addr + mbt->mmap_length)
    {
        dump_memory_map(mmap);
        mmap = (memory_map_t*) ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
    }
}
