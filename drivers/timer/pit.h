#ifndef PIT_H
#define PIT_H

void pit_init(uint32_t frequency);
void pit_handler(void);
uint64_t pit_get_ticks(void);
#endif