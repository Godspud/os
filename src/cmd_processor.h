#ifndef CMD_PROCESSOR_H
#define CMD_PROCESSOR_H

void cmd_processor_init(void);
void process_command(const char *input);
int cmd_is_command(const char *input);

#endif