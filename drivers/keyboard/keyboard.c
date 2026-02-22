#include "../../arch/x86_64/io/ports.h"
#include "../../terminal/terminal.h"
#include "keyboard.h"

#define KEYBOARD_PORT 0x60

static int extended = 0;

static const char scancode_table[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\',
    'z','x','c','v','b','n','m',',','.','/', 0,'*',0,' ',
};

void keyboard_handler(void)
{
    uint8_t scancode = inb(KEYBOARD_PORT);

    /* Extended key prefix */
    if (scancode == 0xE0) {
        extended = 1;
        return;
    }

    /* Ignore key releases (high bit set) */
    if (scancode & 0x80)
        return;

    /* Handle extended keys */
    if (extended) {
        extended = 0;

        switch (scancode) {
            case 0x48:  /* Up arrow */
                terminal_input(KEY_UP);
                return;

            case 0x50:  /* Down arrow */
                terminal_input(KEY_DOWN);
                return;

            default:
                return;
        }
    }

    /* Normal keys */
    if (scancode < 128) {
        char c = scancode_table[scancode];

        if (c)
            terminal_input(c);
    }
}

void keyboard_init(void)
{
    /* Nothing needed yet */
}