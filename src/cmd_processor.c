#include "cmds_processor.h"
#include "commands/default_cmds.h"
#include "string.h"
#include "vga.h"

void cmd_processor_init(void)
{
    // Initialize any necessary data structures or state here
}

void process_command(const char *input)
{
    // Tokenize the input into command and arguments
    char *argv[16];
    int argc = 0;
    char input_copy[256];
    strncpy(input_copy, input, 255);
    input_copy[255] = '\0';

    char *token = strtok(input_copy, " ");
    while (token != 0 && argc < 16)
    {
        argv[argc++] = token;
        token = strtok(0, " ");
    }

    if (argc == 0)
        return;

    // Check for built-in commands
    if (strcmp(argv[0], "help") == 0)
        default_cmds.help(argc, argv);
    else if (strcmp(argv[0], "clear") == 0)
        default_cmds.clear(argc, argv);
    else if (strcmp(argv[0], "echo") == 0)
        default_cmds.echo(argc, argv);
    else if (strcmp(argv[0], "color") == 0)
        default_cmds.color(argc, argv);
    else if (strcmp(argv[0], "version") == 0)
        default_cmds.version(argc, argv);
    else if (strcmp(argv[0], "reboot") == 0)
        default_cmds.reboot(argc, argv);
    else if (strcmp(argv[0], "delay") == 0)
        default_cmds.delay(argc, argv);
    else if (strcmp(argv[0], "rate") == 0)
        default_cmds.rate(argc, argv);
    else
        print_string("Unknown command. Type 'help' for a list of commands.\n", COLOR_LIGHT_RED);
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
