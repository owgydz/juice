
ARCH        = i386
CC          = gcc
LD          = ld
AS          = nasm

CFLAGS      = -ffreestanding -O2 -Wall -Wextra -std=gnu99
LDFLAGS     = -T linker.ld -nostdlib

SRC_C = \
kernel/kernel.c \
kernel/panic.c \
lib/string.c \
lib/memory.c \
mm/pmm.c \
mm/paging.c \
drivers/video/console.c \
drivers/video/vga_text.c \
drivers/video/framebuffer.c \
drivers/keyboard/keyboard.c \
drivers/timer/pit.c \
arch/x86_64/io/ports.c \
arch/x86_64/cpu/gdt.c \
arch/x86_64/cpu/idt.c \
arch/x86_64/cpu/irq.c \
terminal/terminal.c \
terminal/shell.c

SRC_ASM = \
arch/x86_64/boot/boot.asm \
arch/x86_64/cpu/isr.asm

OBJ = $(SRC_C:.c=.o) $(SRC_ASM:.asm=.o)


all: juice.iso

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) -f elf32 $< -o $@

juice.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o juice.bin $(OBJ)

juice.iso: juice.bin
	mkdir -p iso/boot/grub
	cp juice.bin iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o juice.iso iso

run: juice.iso
	qemu-system-i386 -cdrom juice.iso

clean:
	rm -rf *.o */*.o */*/*.o juice.bin juice.iso iso/boot/juice.bin

.PHONY: all clean run