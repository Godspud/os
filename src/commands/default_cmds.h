#ifndef COMMANDS_H
#define COMMANDS_H

typedef void (*cmd_handler)(int argc, char** argv);
typedef struct {
    cmd_handler help;
    cmd_handler clear;
    cmd_handler echo;
    cmd_handler color;
    cmd_handler version;
    cmd_handler reboot;
    cmd_handler delay;
    cmd_handler rate;
} commands;

extern commands default_cmds;

void process_command(const char *input);

#endif