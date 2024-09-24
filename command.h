#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_COMMANDS 16

typedef bool (*cmd_func)(uint8_t argc, char *argv[]);

bool cmd_register(const char *name, const cmd_func f);
bool cmd_execute(char *cmd_str);

#endif // COMMAND_H
