#include <avr/io.h>
#include <stdio.h>

#include "uart.h"
#include "console.h"
#include "command.h"
#include "token.h"
#include "dio.h"
#include "timer.h"
#include "led.h"

enum io_channel {
    LED_GREEN,
    LED_RED,
    NUM_LEDS
};

struct io_info io_cfg[NUM_LEDS] = {
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

struct led_config leds_cfg[NUM_LEDS] = {
    [LED_GREEN] = {
        .name = "green",
        .io_channel = LED_GREEN,
        .blink_period = 100
    },

    [LED_RED] = {
        .name = "red",
        .io_channel = LED_RED,
        .blink_period = 300
    }
};

int main(void)
{
    uart_init();
    console_init();
    timer_init();
    dio_init(io_cfg, NUM_LEDS);
    leds_init(leds_cfg, NUM_LEDS);

    while (1) {
        uart_update();
        console_update();
        timer_update();
    }
}
