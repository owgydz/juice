#include "string.h"

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

int strcmp(const char* a, const char* b)
{
    while (*a && *b) {
        if (*a != *b)
            return *a - *b;
        a++;
        b++;
    }
    return *a - *b;
}

char* itoa(uint32_t value, char* str)
{
    int i = 0;

    if (value == 0) {
        str[i++] = '0';
        str[i] = 0;
        return str;
    }

    while (value > 0) {
        str[i++] = '0' + (value % 10);
        value /= 10;
    }

    str[i] = 0;

    /* reverse */
    for (int j = 0; j < i / 2; j++) {
        char tmp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = tmp;
    }

    return str;
}