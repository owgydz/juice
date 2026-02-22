# The Juice Project

Juice is a small hobby operating system kernel built from scratch for learning and experimentation.

It is designed to start simple while maintaining a clean architecture that can scale into a more serious system over time, like a desktop OS 

---

## ✨ Features

- Multiboot2 (GRUB) boot
- 32-bit protected mode
- GDT + IDT
- PIC remapping + IRQ handling
- PIT timer (uptime tracking)
- PS/2 keyboard driver
- Console abstraction layer
- VGA text backend
- Physical memory manager (bitmap-based)
- Identity-mapped paging
- Interactive shell with:
  - Command history
  - Memory diagnostics
  - Uptime
  - Panic info
  - Page stress test

---

## Building

### Requirements

- `i686-elf-gcc`
- `nasm`
- `grub-mkrescue`
- `qemu-system-i386`

### Build ISO

```bash
make
```

### Run in QEMU
```bash
make
```

