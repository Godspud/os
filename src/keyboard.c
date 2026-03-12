#include "keyboard.h"
#include "io.h"

unsigned int keyboard_repeat_delay = 2000;
unsigned int keyboard_repeat_rate = 150;

static int repeat_enabled = 1;
static int last_scancode = 0;
static int last_key_pressed = 0;
static unsigned int key_hold_time = 0;
static unsigned int last_repeat_time = 0;

static void delay_ms(unsigned int ms)
{
    volatile unsigned int i;
    for (unsigned int m = 0; m < ms; m++)
    {
        for (i = 0; i < 100000; i++)
            ;
    }
}

static unsigned int get_time_ms(void)
{
    return 0;
}

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

    while ((inb(KEYBOARD_STATUS_PORT) & 0x01) != 0)
    {
        inb(KEYBOARD_DATA_PORT);
    }

    last_scancode = 0;
    last_key_pressed = 0;
    key_hold_time = 0;
    last_repeat_time = 0;
}

void keyboard_set_repeat(int enabled)
{
    repeat_enabled = enabled;
}

void keyboard_set_repeat_delay(unsigned int delay_ms)
{
    keyboard_repeat_delay = delay_ms;
}

void keyboard_set_repeat_rate(unsigned int rate_ms)
{
    keyboard_repeat_rate = rate_ms;
}

char keyboard_read(int *shift_pressed)
{
    static int internal_shift = 0;
    static unsigned int repeat_counter = 0;

    if ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0)
    {
        if (repeat_enabled && last_key_pressed && last_scancode != 0)
        {
            repeat_counter++;

            if (repeat_counter > keyboard_repeat_delay / 10)
            {
                if (repeat_counter % (keyboard_repeat_rate / 10) == 0)
                {
                    char repeated_char = scancode_to_ascii(last_scancode, internal_shift);
                    if (repeated_char != 0)
                    {
                        return repeated_char;
                    }
                }
            }
        }
        return 0;
    }

    unsigned char scancode = keyboard_wait();

    if (scancode & 0x80)
    {
        int released_code = scancode & 0x7F;

        if (released_code == 0x2A || released_code == 0x36)
        {
            internal_shift = 0;
            *shift_pressed = 0;
        }

        if (released_code == last_scancode)
        {
            last_key_pressed = 0;
            repeat_counter = 0;
        }

        return 0;
    }

    if (scancode == 0x2A || scancode == 0x36)
    {
        internal_shift = 1;
        *shift_pressed = 1;
        return 0;
    }

    last_scancode = scancode;
    last_key_pressed = 1;
    repeat_counter = 0;

    return scancode_to_ascii(scancode, internal_shift);
}