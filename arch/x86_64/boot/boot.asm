
BITS 32

section .multiboot
align 8
multiboot_header:
    dd 0xE85250D6                
    dd 0                         
    dd header_end - multiboot_header
    dd -(0xE85250D6 + 0 + (header_end - multiboot_header))

    dw 0                         
    dw 0
    dd 8                         

header_end:

section .bss
align 16
stack_bottom:
    resb 16384                   
stack_top:

section .text
global _start
extern kernel_main

_start:
    cli

    ; Setup stack
    mov esp, stack_top

    ; Call kernel
    call kernel_main

.hang:
    hlt
    jmp .hang