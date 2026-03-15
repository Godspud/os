#include "cmd_processor.h"
#include "commands/default_cmds.h"
#include "string.h"
#include "vga.h"

/**
 * str_copy: Copies a string from src to dest, ensuring that it does not exceed max characters and is null-terminated.
 * parse_args: Parses a command line input into individual arguments, splitting on spaces and newlines, and stores them in the provided argv array. Returns the number of arguments parsed.
 * cmd_processor_init: Initializes the command processor (currently does nothing but can be used for future setup).
 * process_command: Takes a command line input, parses it into arguments, and executes the corresponding command handler if it matches a known command. If the command is unknown, it prints an error message.
 * cmd_is_command: Checks if the given input string starts with a known command keyword, returning 1 if it does and 0 otherwise.
 */
// Custom string tokenization (simple space splitter)
int parse_args(const char *input, char argv[][64], int max_args)
{
    int argc = 0;
    int i = 0;
    int j = 0;
    int in_arg = 0;
    while (input[i] != '\0' && argc < max_args)
    {
        if (input[i] == ' ' || input[i] == '\n' || input[i] == '\t')
        {
            if (in_arg)
            {
                argv[argc][j] = '\0';
                argc++;
                j = 0;
                in_arg = 0;
            }
        }
        else
        {
            if (!in_arg)
            {
                in_arg = 1;
            }
            if (j < 63)
            {
                argv[argc][j++] = input[i];
            }
        }
        i++;
    }
    if (in_arg)
    {
        argv[argc][j] = '\0';
        argc++;
    }
    return argc;
}

void cmd_processor_init(void)
{
    // Nothing to initialize yet
}

void process_command(const char *input)
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