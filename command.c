#include <stdint.h>
#include <stdbool.h>

#include "command.h"
#include "token.h"

static struct cmd_info commands[MAX_COMMANDS];
static uint8_t num_commands = 0;

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

bool cmd_execute(char *cmd_str)
{
    struct tokens t = tokenize(cmd_str);

    if (t.num_tokens == 0) {
        return false;
    }

    for (uint8_t i = 0; i < num_commands; i++) {
        if (scmp(commands[i].name, t.list[0])) {
            return commands[i].function(t.num_tokens - 1, t.list + 1);
        }
    }

    return false;
}
