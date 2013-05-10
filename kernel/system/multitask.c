#include <system.h>
#include <context.h>

void tasking_init()
{
    context_t main_context;
    context_save(&main_context);
    kprintf("eip: 0x%8x\n", main_context.eip);
}
