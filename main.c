#include <avr/io.h>
#include <stdio.h>

#include "uart.h"
#include "console.h"
#include "command.h"
#include "token.h"
#include "dio.h"

// user defines input and outputs
enum io_channels {
    LED_GREEN,
    NUM_IO_CHANNELS
};

// user builds a config structure for each io channel
struct io_info io_cfg[NUM_IO_CHANNELS] = {
    [LED_GREEN] = {
        .dat_reg = &PORTB,
        .dir_reg = &DDRB,
        .pin_reg = &PINB,
        .pin = PORTB4,
        .output = true
    }
};

bool led_command(uint8_t argc, char *argv[])
{
    if (scmp("on", argv[1])) {
        dio_wr(LED_GREEN, 1);
    } else if (scmp("off", argv[1])) {
        dio_wr(LED_GREEN, 0);
    }

    return true;
}

int main(void)
{
    uart_init();
    console_init();
    dio_init(io_cfg, NUM_IO_CHANNELS);

    cmd_register("led", led_command);

    while (1) {
        uart_update();
        console_update();
    }
}
