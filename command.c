#include <stdint.h>
#include <stdbool.h>

#include "command.h"
#include "token.h"

static struct cmd_info commands[MAX_COMMANDS];
static uint8_t num_commands = 0;

// called by modules that want to register a command
//
// module provides a name and the function performing the command's actions
// see command.h for the cmd_func prototype
//
// registered commands are stored in commands[] array as 
// cmd_info structs
//
// see command.h for the cmd_info struct definition
bool cmd_register(const char *name, cmd_func f)
{
    if (name && f && (num_commands < MAX_COMMANDS)) {
        commands[num_commands].name = name;
        commands[num_commands].function = f;
        num_commands++;
        return true;
    }

    return false;
}

// called when ready to execute a command string
//
// command string is assumed to be in the form:
// command_name arg1 arg1 ...
//
// searches the registered commands commands[] array for a cmd_info
// struct who's name matches the first token of cmd_str
//
// executes the cmd_info's function pointer, passing it the rest
// of the tokens as arguments
bool cmd_execute(char *cmd_str)
{
    struct tokens t = tokenize(cmd_str);

    if (t.num_tokens == 0) {
        return false;
    }

    for (uint8_t i = 0; i < num_commands; i++) {
        if (scmp(t.list[0], commands[i].name)) {
            return commands[i].function(t.num_tokens, t.list);
        }
    }

    return false;
}
