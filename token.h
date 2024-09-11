#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_TOKENS 5

struct tokens {
    char *list[MAX_TOKENS];
    uint8_t num_tokens;
};

struct tokens tokenize(char *str);
bool scmp(const char *s1, const char *s2);

#endif // TOKEN_H
