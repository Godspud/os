#include "vga.h"
#include "io.h"
#include "string.h"

int cursor_x = 0;
int cursor_y = 0;
static int cursor_enabled = 1;

void vga_write_char(int x, int y, char c, int color)
{
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT)
        return;
    unsigned short *vga = (unsigned short *)VGA_MEMORY;
    int offset = y * VGA_WIDTH + x;
    vga[offset] = (color << 8) | (unsigned char)c;
}

void vga_write_string(int x, int y, const char *str, int color)
{
    int i = 0;
    while (str[i] != '\0')
    {
        vga_write_char(x + i, y, str[i], color);
        i++;
    }
}

void vga_clear(int bgcolor)
{
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            vga_write_char(x, y, ' ', bgcolor << 8);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    vga_move_cursor();
}

void vga_move_cursor(void)
{
    if (!cursor_enabled)
        return;
    int pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);

    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

void vga_disable_cursor()
{
    cursor_enabled = 0;
    outb(0x3D4, 10);
    outb(0x3D5, 0x20);
}

void vga_enable_cursor(void)
{
    cursor_enabled = 1;
    outb(0x3D4, 10);
    outb(0x3D5, 14);
    outb(0x3D4, 11);
    outb(0x3D5, 15);
    vga_move_cursor();
}

void print_char(char c, int color)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else if (c == '\b')
    {
        if (cursor_x > 0)
        {
            cursor_x--;
            vga_write_char(cursor_x, cursor_y, ' ', color);
        }
        else if (cursor_y > 0)
        {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
            vga_write_char(cursor_x, cursor_y, ' ', color);
        }
        vga_move_cursor();
        return;
    }
    else if (c == '\t')
    {
        cursor_x = (cursor_x + 4) & ~3;
    }
    else if (c >= 32 && c <= 126)
    {
        vga_write_char(cursor_x, cursor_y, c, color);
        cursor_x++;
    }
    if (cursor_x >= VGA_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT)
    {
        vga_scroll();
        cursor_y = VGA_HEIGHT - 1;
    }

    vga_move_cursor();
}

void vga_scroll()
{
    unsigned short *vga = (unsigned short *)VGA_MEMORY;
    for (int y = 0; y < VGA_HEIGHT - 1; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            int src = (y + 1) * VGA_WIDTH + x;
            int dst = y * VGA_WIDTH + x;
            vga[dst] = vga[src];
        }
    }
    int last_line = (VGA_HEIGHT - 1) * VGA_WIDTH;
    for (int x = 0; x < VGA_WIDTH; x++)
    {
        vga[last_line + x] = (COLOR_BLACK << 8) | ' ';
    }
}

void print_string(const char *str, int color)
{
    int i = 0;
    while (str[i] != '\0')
    {
        print_char(str[i], color);
        i++;
    }
}