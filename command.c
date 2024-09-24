#include <stdint.h>
#include <stdbool.h>

#include "command.h"
#include "token.h"

struct cmd_info {
    const char *name;
    cmd_func function;
};

static struct cmd_info commands[MAX_COMMANDS];
static uint8_t num_commands = 0;

bool cmd_register(const char *name, cmd_func f)
{
    if (!name || !f || (num_commands >= MAX_COMMANDS)) {
        return false;
    }

    commands[num_commands].name = name;
    commands[num_commands].function = f;
    num_commands++;

    return true;
}

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
