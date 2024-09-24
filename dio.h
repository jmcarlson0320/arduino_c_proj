#ifndef DIO_H
#define DIO_H

#include <stdint.h>
#include <stdbool.h>

struct io_info {
    // the three registers associated with a port
    volatile uint8_t *dat_reg;
    volatile uint8_t *dir_reg;
    volatile uint8_t *pin_reg;

    // the pin number to use for this IO channel
    volatile uint8_t pin;

    // whether to use channel as input or output
    bool output;
};

void dio_init(struct io_info *cfg, uint8_t num_io_info);
void dio_wr(uint8_t io_idx, bool val);
bool dio_rd(uint8_t io_idx);

#endif // DIO_H
