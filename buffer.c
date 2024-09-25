#include "buffer.h"

struct buffer buffer_init(uint8_t *byte_array, uint16_t size)
{
    struct buffer b;

    b.buf = byte_array;
    b.size = size;
    b.put_idx = 0;
    b.get_idx = 0;
    b.count = 0;

    return b;
}

int8_t buffer_get(struct buffer *b, uint8_t *byte)
{
    if (b->count == 0) {
        return BUFFER_ERROR;
    }

    *byte = b->buf[b->get_idx++];
    b->get_idx %= b->size;

    b->count--;

    return BUFFER_SUCCESS;
}

int8_t buffer_put(struct buffer *b, uint8_t byte)
{
    if (b->count == b->size) {
        return BUFFER_ERROR;
    }

    b->buf[b->put_idx++] = byte;
    b->put_idx %= b->size;

    b->count++;

    return BUFFER_SUCCESS;
}
