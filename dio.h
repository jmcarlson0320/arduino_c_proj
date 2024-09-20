#ifndef DIO_H
#define DIO_H

struct io_info {
    // properties here
    volatile uint8_t *dat_reg;
    volatile uint8_t *dir_reg;
    volatile uint8_t *pin_reg;
    volatile uint8_t pin;
    bool output;
};

void dio_init(struct io_info *cfg, uint8_t num_io_info);
void dio_wr(uint8_t io_idx, bool val);
bool dio_rd(uint8_t io_idx);

#endif // DIO_H
