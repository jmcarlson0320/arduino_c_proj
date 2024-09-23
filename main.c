#include <avr/io.h>
#include <stdio.h>

#include "uart.h"
#include "console.h"
#include "command.h"
#include "token.h"
#include "dio.h"
#include "timer.h"

// user defines input and outputs
enum io_channels {
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

static uint8_t red_state = 0;
static uint8_t red_timer;
enum timer_action red_callback(void)
{
    red_state = ~red_state;
    dio_wr(LED_RED, red_state);

    return TIMER_ACTION_REPEAT;
}

static uint8_t green_state = 0;
static uint8_t green_timer;
enum timer_action green_callback(void)
{
    green_state = ~green_state;
    dio_wr(LED_GREEN, green_state);

    return TIMER_ACTION_REPEAT;
}

bool led_command(uint8_t argc, char *argv[])
{
    enum io_channels channel;
    uint8_t timer;
    uint16_t ms;

    if (argc != 3) {
        printf("usage: led <color> <on/off>\n");
        return false;
    }

    if (scmp("red", argv[1])) {
        channel = LED_RED;
        timer = red_timer;
        ms = 250;
    } else if (scmp("green", argv[1])) {
        channel = LED_GREEN;
        timer = green_timer;
        ms = 100;
    } else {
        printf("invalid led name\n");
        printf("usage: led <color> <on/off>\n");
        return false;
    }

    if (scmp("on", argv[2])) {
        dio_wr(channel, 1);
        timer_stop(timer);
    } else if (scmp("off", argv[2])) {
        dio_wr(channel, 0);
        timer_stop(timer);
    } else if (scmp("blink", argv[2])) {
        timer_start(timer, ms);
    } else {
        printf("invalid control description\n");
        printf("usage: led <color> <on/off>\n");
        return false;
    }

    return true;
}

bool read_io(uint8_t argc, char *argv[])
{
    bool val = dio_rd(D7_IN);

    if (val) {
        printf("value of dio input 7 is HIGH\n");
    } else {
        printf("value of dio input 7 is LOW\n");
    }

    return true;
}

int main(void)
{
    uart_init();
    console_init();
    timer_init();
    dio_init(io_cfg, NUM_IO_CHANNELS);

    cmd_register("led", led_command);
    cmd_register("read", read_io);

    red_timer = timer_get();
    timer_set_callback(red_timer, red_callback);

    green_timer = timer_get();
    timer_set_callback(green_timer, green_callback);

    while (1) {
        uart_update();
        console_update();
        timer_update();
    }
}
