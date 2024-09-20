#include <stdint.h>
#include <stdbool.h>

#include "dio.h"

static struct io_info *io_channels;
static uint8_t num_channels;

static void init_channel(struct io_info channel)
{
    if (channel.output) {
        *(channel.dir_reg) |= (1 << channel.pin);
    }
}

void dio_init(struct io_info *cfg, uint8_t num_io_info)
{
    io_channels = cfg;
    num_channels = num_io_info;

    for (uint8_t i = 0; i < num_io_info; i++) {
        init_channel(io_channels[i]);
    }
}

// main api for setting io
void dio_wr(uint8_t io_idx, bool val)
{
    if (io_idx < 0 || num_channels <= io_idx) {
        return;
    }

    if (val) {
        *(io_channels[io_idx].dat_reg) |= (1 << io_channels[io_idx].pin);
    } else {
        *(io_channels[io_idx].dat_reg) &= ~(1 << io_channels[io_idx].pin);
    }
}
