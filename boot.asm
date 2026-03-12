[BITS 32]

extern kernel_main

magic    equ 0x1BADB002
flags    equ 0x0
checksum equ -(magic + flags)

section .multiboot
    dd magic
    dd flags
    dd checksum

section .text
    global start

start:
    cli
    call kernel_main
    hlt