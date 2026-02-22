#include <stdint.h>

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;
static uint8_t color = 0x0F;
static int cursor_row = 0;
static int cursor_col = 0;

static void vga_put_char(char c)
{
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        return;
    }

    int index = cursor_row * VGA_WIDTH + cursor_col;
    vga_buffer[index] = (color << 8) | c;

    cursor_col++;
    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
    }
}

void vga_init(void)
{
    vga_clear();
}

void vga_clear(void)
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga_buffer[i] = (color << 8) | ' ';

    cursor_row = 0;
    cursor_col = 0;
}

void vga_write(const char* str)
{
    while (*str)
        vga_put_char(*str++);
}