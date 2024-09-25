#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

#define BUFFER_ERROR -1
#define BUFFER_SUCCESS 0

struct buffer {
    uint8_t *buf;
    uint16_t size;
    uint16_t put_idx;
    uint16_t get_idx;
    uint16_t count;
};

struct buffer buffer_init(uint8_t *byte_array, uint16_t size);
int8_t buffer_get(struct buffer *b, uint8_t *byte);
int8_t buffer_put(struct buffer *b, uint8_t byte);

#endif // BUFFER_H
