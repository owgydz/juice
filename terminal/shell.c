#include "../drivers/video/console.h"
#include "../lib/string.h"
#include "../mm/pmm.h"
#include "../drivers/timer/pit.h"
#include "../kernel/panic.h"
#include "shell.h"

#define MAX_ARGS 8

/* ===========================
   Utilities
   =========================== */

static void print_prompt(void)
{
    console_write("\njuice> ");
}

static int parse_args(char* input, char* argv[])
{
    int argc = 0;

    while (*input && argc < MAX_ARGS) {

        /* Skip spaces */
        while (*input == ' ')
            input++;

        if (!*input)
            break;

        argv[argc++] = input;

        /* Find end of word */
        while (*input && *input != ' ')
            input++;

        if (*input) {
            *input = '\0';
            input++;
        }
    }

    return argc;
}


static void cmd_help(int argc, char** argv)
{
    (void)argc; (void)argv;

    console_write(
        "\nAvailable commands:\n"
        " help       - Show this list\n"
        " clear      - Clear screen\n"
        " about      - About Juice\n"
        " mem        - Memory usage\n"
        " pages      - Page statistics\n"
        " echo       - Echo text\n"
        " version    - Kernel version\n"
        " stress      - Memory stress test\n"
        " uptime      - Show system uptime\n"
    );
}

static void cmd_clear(int argc, char** argv)
{
    (void)argc; (void)argv;
    console_clear();
}

static void cmd_about(int argc, char** argv)
{
    (void)argc; (void)argv;
    console_write(
        "\nJuice Kernel\n"
        "Juice is a small hobby kernel with real architecture.\n"
    );
}

static void cmd_version(int argc, char** argv)
{
    (void)argc; (void)argv;
    console_write("\nJuice v0.01-standard\n");
    console_write("UUID: 8994ffe4-8547-4da8-a0c5-e2cb2ed6568e\n");
}

static void cmd_mem(int argc, char** argv)
{
    (void)argc; (void)argv;

    uint32_t total = pmm_get_total_pages();
    uint32_t used  = pmm_get_used_pages();
    uint32_t free  = total - used;

    console_write("\nMemory Info:\n");

    /* crude number printing */
    char buf[32];

    console_write(" Total pages: ");
    itoa(total, buf);
    console_write(buf);

    console_write("\n Used pages: ");
    itoa(used, buf);
    console_write(buf);

    console_write("\n Free pages: ");
    itoa(free, buf);
    console_write(buf);

    console_write("\n");
}

static void cmd_pages(int argc, char** argv)
{
    (void)argc; (void)argv;

    char buf[32];
    console_write("\nTotal pages: ");
    itoa(pmm_get_total_pages(), buf);
    console_write(buf);
    console_write("\n");
}

static void cmd_echo(int argc, char** argv)
{
    console_write("\n");

    for (int i = 1; i < argc; i++) {
        console_write(argv[i]);
        console_write(" ");
    }

    console_write("\n");
}

static void cmd_stress(int argc, char** argv)
{
    (void)argc; (void)argv;

    console_write("\nAllocating pages...\n");

    void* pages[256];
    int count = 0;

    for (int i = 0; i < 256; i++) {
        pages[i] = pmm_alloc_page();
        if (!pages[i])
            break;
        count++;
    }

    char buf[32];
    console_write("Allocated: ");
    itoa(count, buf);
    console_write(buf);
    console_write(" pages\n");

    for (int i = 0; i < count; i++)
        pmm_free_page(pages[i]);

    console_write("Freed pages.\n");
}

static void cmd_uptime(int argc, char** argv)
{
    (void)argc; (void)argv;

    uint64_t ticks = pit_get_ticks();
    uint64_t seconds = ticks / 100;

    char buf[32];

    console_write("\nUptime: ");
    itoa((uint32_t)seconds, buf);
    console_write(buf);
    console_write(" seconds\n");
}

#include "../kernel/panic.h"

static void cmd_panicinfo(int argc, char** argv)
{
    (void)argc; (void)argv;

    const char* msg = kernel_last_panic();

    if (!msg) {
        console_write("\nNo panic recorded.\n");
        return;
    }

    console_write("\nLast panic:\n");
    console_write(msg);
    console_write("\n");
}

typedef struct {
    const char* name;
    void (*func)(int argc, char** argv);
} command_t;

static command_t commands[] = {
    { "help",    cmd_help },
    { "clear",   cmd_clear },
    { "about",   cmd_about },
    { "version", cmd_version },
    { "mem",     cmd_mem },
    { "pages",   cmd_pages },
    { "echo",    cmd_echo },
    { "stress", cmd_stress },
    { "uptime", cmd_uptime },
    { "panicinfo", cmd_panicinfo },
};

#define COMMAND_COUNT (sizeof(commands)/sizeof(commands[0]))


void shell_execute(const char* input)
{
    char buffer[128];
    char* argv[MAX_ARGS];

    /* Make modifiable copy */
    int len = strlen(input);
    if (len >= 127)
        len = 127;

    for (int i = 0; i < len; i++)
        buffer[i] = input[i];

    buffer[len] = '\0';

    int argc = parse_args(buffer, argv);

    if (argc == 0)
        return;

    for (uint32_t i = 0; i < COMMAND_COUNT; i++) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            commands[i].func(argc, argv);
            return;
        }
    }

    console_write("\nUnknown command. Type 'help'.\n");
}