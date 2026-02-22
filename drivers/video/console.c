#include "console.h"
#include "vga_text.h"

void console_init(void)
{
    vga_init();
}

void console_write(const char* str)
{
    vga_write(str);
}

void console_clear(void)
{
    vga_clear();
}