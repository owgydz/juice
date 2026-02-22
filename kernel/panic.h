#ifndef PANIC_H
#define PANIC_H

void kernel_panic(const char* msg);
const char* kernel_last_panic(void);

#endif