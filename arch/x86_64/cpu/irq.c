#include "idt.h"
#include "../io/ports.h"
#include "../../../drivers/keyboard/keyboard.h"
#include "../../../drivers/timer/pit.h"

#define PIC1 0x20
#define PIC2 0xA0

static const char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating Point",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point",
    "Virtualization",
    "Control Protection",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection",
    "VMM Communication",
    "Security Exception",
    "Reserved"
};

void isr_handler(registers_t* r)
{
    extern void console_write(const char*);

    console_write("\nCPU Exception: ");
    console_write(exception_messages[r->int_no]);
    console_write("\nSystem Halted.\n");

    while (1);
}

void irq_handler(registers_t* r)
{
    uint8_t irq = r->int_no - 32;

    switch (irq) {
        case 0:
            pit_handler();
            break;
        case 1:
            keyboard_handler();
            break;
    }

    if (irq >= 8)
        outb(PIC2, 0x20);

    outb(PIC1, 0x20);
}