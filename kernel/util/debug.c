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
