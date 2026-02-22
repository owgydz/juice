
#include <stdint.h>

#include "../arch/x86_64/boot/multiboot2.h"
#include "../arch/x86_64/cpu/gdt.h"
#include "../arch/x86_64/cpu/idt.h"

#include "../drivers/video/console.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/timer/pit.h"

#include "../mm/pmm.h"
#include "../mm/paging.h"

#include "../terminal/terminal.h"

void kernel_main(uint32_t magic, uint32_t addr)
{

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        while (1);
    }

    gdt_init();
    idt_init();

    multiboot_tag_t* tag;
    multiboot_tag_mmap_t* mmap = 0;

    for (tag = (multiboot_tag_t*)(addr + 8);
         tag->type != 0;
         tag = (multiboot_tag_t*)((uint8_t*)tag + ((tag->size + 7) & ~7)))
    {
        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
            mmap = (multiboot_tag_mmap_t*)tag;
            break;
        }
    }

    if (!mmap) {
        while (1);
    }

    pmm_init(mmap);
    paging_init();
    console_init();
    keyboard_init();
    pit_init(100);  /* 100 Hz timer */

    /* Enable interrupts */
    __asm__ volatile ("sti");

    terminal_init();

    while (1) {
        __asm__ volatile ("hlt");
    }
}