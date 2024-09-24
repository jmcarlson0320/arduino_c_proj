#include <avr/io.h>
#include <stdio.h>

#include "uart.h"
#include "console.h"
#include "command.h"
#include "token.h"
#include "dio.h"
#include "timer.h"

// TODO
// - refactor led code into a module
// - add userdata to callback
// - use a single callback, check which led by userdata

// user defines input and outputs
enum io_channel {
    LED_GREEN,
    LED_RED,
    D7_IN,
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
    },

    [D7_IN] = {
        .dat_reg = &PORTD,
        .dir_reg = &DDRD,
        .pin_reg = &PIND,
        .pin = PORTD7,
        .output = false
    }
};

struct led_info {
    uint8_t state;
    uint8_t timer_id;
    enum io_channel channel;
    uint16_t blink_period;
};

enum led_label {
    RED,
    GREEN,
    NUM_LEDS
};

static struct led_info leds[NUM_LEDS];

enum timer_action red_callback(void)
{
    leds[RED].state = ~leds[RED].state;
    dio_wr(leds[RED].channel, leds[RED].state);

    return TIMER_ACTION_REPEAT;
}

enum timer_action green_callback(void)
{
    leds[GREEN].state = ~leds[GREEN].state;
    dio_wr(leds[GREEN].channel, leds[GREEN].state);

    return TIMER_ACTION_REPEAT;
}

bool led_command(uint8_t argc, char *argv[])
{
    enum led_label idx;

    if (argc != 3) {
        printf("usage: led <color> <on/off/blink>\n");
        return false;
    }

    if (scmp("red", argv[1])) {
        idx = RED;
    } else if (scmp("green", argv[1])) {
        idx = GREEN;
    } else {
        printf("invalid led name\n");
        printf("usage: led <color> <on/off/blink>\n");
        return false;
    }

    if (scmp("on", argv[2])) {
        dio_wr(leds[idx].channel, 1);
        timer_stop(leds[idx].timer_id);
    } else if (scmp("off", argv[2])) {
        dio_wr(leds[idx].channel, 0);
        timer_stop(leds[idx].timer_id);
    } else if (scmp("blink", argv[2])) {
        timer_start(leds[idx].timer_id, leds[idx].blink_period);
    } else {
        printf("invalid control description\n");
        printf("usage: led <color> <on/off/blink>\n");
        return false;
    }

    return true;
}

void leds_init(void)
{
    leds[RED].state = 0;
    leds[RED].blink_period = 250;
    leds[RED].channel = LED_RED;
    leds[RED].timer_id = timer_get();
    timer_set_callback(leds[RED].timer_id, red_callback);

    leds[GREEN].state = 0;
    leds[GREEN].blink_period = 100;
    leds[GREEN].channel = LED_GREEN;
    leds[GREEN].timer_id = timer_get();
    timer_set_callback(leds[GREEN].timer_id, green_callback);

    cmd_register("led", led_command);
}

int main(void)
{
    uart_init();
    console_init();
    timer_init();
    dio_init(io_cfg, NUM_IO_CHANNELS);

    leds_init();

    while (1) {
        uart_update();
        console_update();
        timer_update();
    }
}
