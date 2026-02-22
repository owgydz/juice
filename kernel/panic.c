#include "../drivers/video/console.h"

static const char* last_panic = 0;

void kernel_panic(const char* msg)
{
    last_panic = msg;

    console_write("\n=== KERNEL PANIC ===\n");
    console_write(msg);
    console_write("\nThere's been a kernel panic. The system is halted.\n");

    while (1);
}

const char* kernel_last_panic(void)
{
    return last_panic;
}