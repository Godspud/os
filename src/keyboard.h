#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

char keyboard_read(int* shift_pressed);

unsigned char keyboard_wait(void);

void keyboard_init(void);

#endif