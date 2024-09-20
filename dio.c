#include <stdint.h>
#include <stdbool.h>

#include "dio.h"

static struct io_info *io_channels;
static uint8_t num_channels;

void dio_init(struct io_info *cfg, uint8_t num_io_info)
{
    io_channels = cfg;
    num_channels = num_io_info;

    for (uint8_t i = 0; i < num_io_info; i++) {
        struct io_info c = io_channels[i];
        if (c.output) {
            // configure pin as output
            *(c.dir_reg) |= (1 << c.pin);
        }
    }
}

// main api for setting io
void dio_wr(uint8_t io_idx, bool val)
{
    if (io_idx < 0 || num_channels <= io_idx) {
        return;
    }

    struct io_info c = io_channels[io_idx];

    if (val) {
        *(c.dat_reg) |= (1 << c.pin);
    } else {
        *(c.dat_reg) &= ~(1 << c.pin);
    }
}

bool dio_rd(uint8_t io_idx)
{
    if (io_idx < 0 || num_channels <= io_idx) {
        return false;
    }

    struct io_info c = io_channels[io_idx];

    return (*(c.pin_reg) & (1 << c.pin));
}
