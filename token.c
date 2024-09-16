#include <stdbool.h>

#include "token.h"

static char *skip_space(char *str)
{
    while (*str == ' ') {
        str++;
    }
    return str;
}

// utility function that tokenizes a string in-place
//
// modifies str by replacing SPACE's with NULL's
//
// returns a struct tokens containing an array of tokens
struct tokens tokenize(char *str)
{
    struct tokens t;
    t.num_tokens = 0;

    str = skip_space(str);

    if (*str) {
        t.list[0] = str;
        t.num_tokens = 1;
    }

    while (*str) {
        if (*str == ' ') {
            *str = '\0';
            if (t.num_tokens == MAX_TOKENS) {
                break;
            }
            str++;
            str = skip_space(str);
            if (*str) {
                t.list[t.num_tokens] = str;
                t.num_tokens++;
            }
        } else {
            str++;
        }
    }
    return t;
}

// utility function for comparing strings
//
// used by the command module when searching for a command
// by name
bool scmp(const char *s1, const char *s2)
{
    while (*s1++ == *s2++) {
        if (*s1 == '\0') {
            return true;
        }
    }
    return false;
}

