#include <multiboot.h>
#include <types.h>
#include <video.h>

void dump_memory_map(memory_map_t* mmap)
{
    put_int(mmap->base_addr_low);
    put_char(' ');
    put_int(mmap->length_low / 1024);
    put_char(' ');
    put_int(mmap->base_addr_high);
    put_char(' ');
    put_int(mmap->length_high / 1024);
    put_char(' ');
    if (mmap->type == 1) {
        put_string("Available");
    } else {
        put_string("Unavailable");
    }
    put_char('\n');
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
