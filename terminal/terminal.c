#include "../drivers/video/console.h"
#include "../lib/string.h"
#include "terminal.h"
#include "shell.h"

#define INPUT_MAX   128
#define HISTORY_MAX 16

static char buffer[INPUT_MAX];
static int  pos = 0;

static char history[HISTORY_MAX][INPUT_MAX];
static int  history_count = 0;
static int  history_cursor = 0;



static void print_prompt(void)
{
    console_write("\njuice> ");
}

static void clear_current_line(void)
{
    console_write("\njuice> ");
}



void terminal_init(void)
{
    console_write("\nWelcome to the Juice v0.01-standard terminal shell\n");
    print_prompt();
}


static void save_history(void)
{
    if (pos == 0)
        return;

    int index = history_count % HISTORY_MAX;
    strcpy(history[index], buffer);

    history_count++;
    history_cursor = history_count;
}

static void load_history(int index)
{
    if (index < 0 || index >= history_count)
        return;

    int real_index = index % HISTORY_MAX;

    console_write("\njuice> ");
    strcpy(buffer, history[real_index]);
    console_write(buffer);

    pos = strlen(buffer);
}


void terminal_input(char c)
{
    /* ----- ENTER ----- */
    if (c == '\n') {
        buffer[pos] = 0;

        save_history();
        shell_execute(buffer);

        pos = 0;
        buffer[0] = 0;

        print_prompt();
        return;
    }

    /* ----- BACKSPACE ----- */
    if (c == KEY_BACKSPACE) {
        if (pos > 0) {
            pos--;
            buffer[pos] = 0;
            console_write("\b \b");
        }
        return;
    }

    /* ----- UP ARROW ----- */
    if (c == KEY_UP) {
        if (history_count == 0)
            return;

        if (history_cursor > 0)
            history_cursor--;

        load_history(history_cursor);
        return;
    }

    /* ----- DOWN ARROW ----- */
    if (c == KEY_DOWN) {
        if (history_cursor < history_count - 1)
            history_cursor++;
        else {
            /* Clear line if at newest */
            console_write("\njuice> ");
            pos = 0;
            buffer[0] = 0;
            return;
        }

        load_history(history_cursor);
        return;
    }

    /* ----- NORMAL CHARACTER ----- */
    if (pos < INPUT_MAX - 1) {
        buffer[pos++] = c;
        buffer[pos] = 0;

        char str[2] = {c, 0};
        console_write(str);
    }
}