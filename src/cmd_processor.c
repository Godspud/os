#include "cmd_processor.h"
#include "commands/default_cmds.h"
#include "string.h"
#include "vga.h"

// Custom string copy (no libc)
static void str_copy(char *dest, const char *src, int max)
{
    int i = 0;
    while (i < max - 1 && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// Custom string tokenization (simple space splitter)
static int parse_args(const char *input, char argv[][64], int max_args)
{
    int argc = 0;
    int i = 0;
    int arg_start = 0;
    int in_arg = 0;

    while (input[i] != '\0' && argc < max_args)
    {
        if (input[i] == ' ' || input[i] == '\n')
        {
            if (in_arg)
            {
                // End of argument
                int len = i - arg_start;
                if (len > 0 && len < 64)
                {
                    int j = 0;
                    while (j < len)
                    {
                        argv[argc][j] = input[arg_start + j];
                        j++;
                    }
                    argv[argc][len] = '\0';
                    argc++;
                }
                in_arg = 0;
            }
        }
        else
        {
            if (!in_arg)
            {
                arg_start = i;
                in_arg = 1;
            }
        }
        i++;
    }

    // Handle last argument
    if (in_arg && argc < max_args)
    {
        int len = i - arg_start;
        if (len > 0 && len < 64)
        {
            int j = 0;
            while (j < len)
            {
                argv[argc][j] = input[arg_start + j];
                j++;
            }
            argv[argc][len] = '\0';
            argc++;
        }
    }

    return argc;
}

void cmd_processor_init(void)
{
    // Nothing to initialize yet
}

void cmd_process(const char *input)
{
    char argv[16][64];
    int argc = 0;

    // Parse input into arguments
    argc = parse_args(input, argv, 16);

    if (argc == 0)
    {
        return;
    }

    // Check for built-in commands
    if (strcmp(argv[0], "help") == 0)
    {
        default_cmds.help(argc, (char **)argv);
    }
    else if (strcmp(argv[0], "clear") == 0)
    {
        default_cmds.clear(argc, (char **)argv);
    }
    else if (strcmp(argv[0], "echo") == 0)
    {
        default_cmds.echo(argc, (char **)argv);
    }
    else if (strcmp(argv[0], "color") == 0)
    {
        default_cmds.color(argc, (char **)argv);
    }
    else if (strcmp(argv[0], "version") == 0)
    {
        default_cmds.version(argc, (char **)argv);
    }
    else if (strcmp(argv[0], "reboot") == 0)
    {
        default_cmds.reboot(argc, (char **)argv);
    }
    else if (strcmp(argv[0], "delay") == 0)
    {
        default_cmds.delay(argc, (char **)argv);
    }
    else if (strcmp(argv[0], "rate") == 0)
    {
        default_cmds.rate(argc, (char **)argv);
    }
    else
    {
        print_string("Unknown command. Type 'help' for a list of commands.\n", COLOR_LIGHT_RED);
    }
}

int cmd_is_command(const char *input)
{
    if (strncmp(input, "help", 4) == 0)
        return 1;
    if (strncmp(input, "clear", 5) == 0)
        return 1;
    if (strncmp(input, "echo", 4) == 0)
        return 1;
    if (strncmp(input, "color", 5) == 0)
        return 1;
    if (strncmp(input, "version", 7) == 0)
        return 1;
    if (strncmp(input, "reboot", 6) == 0)
        return 1;
    if (strncmp(input, "delay", 5) == 0)
        return 1;
    if (strncmp(input, "rate", 4) == 0)
        return 1;
    return 0;
}