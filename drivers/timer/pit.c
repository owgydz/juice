#include "../../arch/x86_64/io/ports.h"
#include <stdint.h>

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40
static volatile uint64_t tick_count = 0;


void pit_init(uint32_t frequency)
{
    uint32_t divisor = 1193180 / frequency;

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}

void pit_handler(void)
{
    tick_count++;
}

uint64_t pit_get_ticks(void)
{
    return tick_count;
}