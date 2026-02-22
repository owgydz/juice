#ifndef TERMINAL_H
#define TERMINAL_H

#define KEY_UP    0x80
#define KEY_DOWN  0x81
#define KEY_BACKSPACE  '\b'

void terminal_init(void);
void terminal_input(char c);

#endif