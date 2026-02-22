
#include "memory.h"

void* memcpy(void* dest, const void* src, size_t count)
{
    unsigned char* d = dest;
    const unsigned char* s = src;

    for (size_t i = 0; i < count; i++)
        d[i] = s[i];

    return dest;
}

void* memset(void* dest, int value, size_t count)
{
    unsigned char* d = dest;

    for (size_t i = 0; i < count; i++)
        d[i] = (unsigned char)value;

    return dest;
}