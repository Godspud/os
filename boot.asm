[BITS 32]

majic equ 0x1BADB002
flags equ 0x0
checksum equ -(majic + flags)

section .muiltiboot
    dd majic
    dd flags
    dd checksum

section .text
    global start

start:
    cli
    call kernel_main
    hlt