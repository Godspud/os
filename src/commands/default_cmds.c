#include "default_cmds.h"
#include "string.h"
#include "vga.h"
#include "keyboard.h"
#include "io.h"

static void cmd_help(int argc, char **argv)
{
    print_string("Available commands:\n", COLOR_LIGHT_CYAN);
    print_string("help - Show this help message\n", COLOR_LIGHT_GREEN);
    print_string("clear - Clear the screen\n", COLOR_LIGHT_GREEN);
    print_string("echo [text] - Print the text back to the screen\n", COLOR_LIGHT_GREEN);
    print_string("color [color] - Change text color (0-15)\n", COLOR_LIGHT_GREEN);
    print_string("version - Show OS version\n", COLOR_LIGHT_GREEN);
    print_string("reboot - Reboot the system\n", COLOR_LIGHT_GREEN);
    print_string("delay [ms] - Set keyboard repeat delay in ms\n", COLOR_LIGHT_GREEN);
    print_string("rate [ms] - Set keyboard repeat rate in ms\n", COLOR_LIGHT_GREEN);
}
static void cmd_clear(int argc, char **argv)
{
    vga_clear(COLOR_BLACK);
}
static void cmd_echo(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        print_string(argv[i], COLOR_WHITE);
        if (i < argc - 1)
            print_string(" ", COLOR_WHITE);
    }
    print_char('\n', COLOR_WHITE);
}
static void cmd_color(int argc, char **argv)
{
    if (argc != 2)
    {
        print_string("Usage: color [0-15]\n", COLOR_LIGHT_RED);
        return;
    }
    int color = atoi(argv[1]);
    if (color < 0 || color > 15)
    {
        print_string("Color must be between 0 and 15\n", COLOR_LIGHT_RED);
        return;
    }
    print_string("Text color changed\n", COLOR_LIGHT_GREEN);
}
static void cmd_version(int argc, char **argv)
{
    print_string("OS Version: 0.1\n", COLOR_LIGHT_CYAN);
}
static void cmd_reboot(int argc, char **argv)
{
    outb(0x64, 0xFE);
}