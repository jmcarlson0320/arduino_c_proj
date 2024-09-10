#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

#define MAX_TOKENS 3

struct tokens {
    void *list[MAX_TOKENS];
    uint8_t num_tokens;
};

struct tokens tokenize(char *str);

#endif // TOKEN_H
