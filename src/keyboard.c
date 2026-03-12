// src/keyboard.c
#include "keyboard.h"
#include "io.h"

char scancode_to_ascii(unsigned char scancode, int shift_pressed)
{
    static const char normal[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
        0, ' '};

    static const char shifted[] = {
        0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
        0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*',
        0, ' '};

    if (scancode >= 59)
        return 0;

    if (shift_pressed)
    {
        return shifted[scancode];
    }
    else
    {
        return normal[scancode];
    }
}

unsigned char keyboard_wait(void)
{
    while ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0)
        ;
    return inb(KEYBOARD_DATA_PORT);
}

void keyboard_init(void)
{
    while ((inb(KEYBOARD_STATUS_PORT) & 0x02) != 0)
        ;

    outb(0x64, 0xAE);

    outb(0x60, 0xF0);
    outb(0x60, 0x02);

    while ((inb(KEYBOARD_STATUS_PORT) & 0x01) != 0)
    {
        inb(KEYBOARD_DATA_PORT);
    }
}

char keyboard_read(int *shift_pressed)
{
    unsigned char scancode = keyboard_wait();

    if (scancode & 0x80)
    {
        if (scancode == 0xAA || scancode == 0xB6)
        {
            *shift_pressed = 0;
        }
        return 0;
    }

    if (scancode == 0x2A || scancode == 0x36)
    {
        *shift_pressed = 1;
        return 0;
    }

    return scancode_to_ascii(scancode, *shift_pressed);
}