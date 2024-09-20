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
    LED_RED,
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
    },

    [LED_RED] = {
        .dat_reg = &PORTB,
        .dir_reg = &DDRB,
        .pin_reg = &PINB,
        .pin = PORTB3,
        .output = true
    }
};

bool led_command(uint8_t argc, char *argv[])
{
    enum io_channels channel;

    if (argc != 3) {
        printf("usage: led <color> <on/off>\n");
        return false;
    }

    if (scmp("red", argv[1])) {
        channel = LED_RED;
    } else if (scmp("green", argv[1])) {
        channel = LED_GREEN;
    } else {
        printf("invalid led name\n");
        printf("usage: led <color> <on/off>\n");
        return false;
    }

    if (scmp("on", argv[2])) {
        dio_wr(channel, 1);
    } else if (scmp("off", argv[2])) {
        dio_wr(channel, 0);
    } else {
        printf("invalid control description\n");
        printf("usage: led <color> <on/off>\n");
        return false;
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
