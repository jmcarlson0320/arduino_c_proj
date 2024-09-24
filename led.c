#include <stdlib.h>
#include <stdio.h>

#include "led.h"
#include "token.h"
#include "timer.h"
#include "command.h"

#define LED_ON 1
#define LED_OFF 0

struct led_info {
    char *name;
    uint8_t io_channel;
    uint16_t blink_period;
    uint8_t timer_id;
    uint8_t state;
};

static struct led_info *leds;
static uint8_t num_leds;

static bool led_command(uint8_t argc, char *argv[])
{
    if (argc != 3) {
        printf("usage: led <color> <on/off/blink>\n");
        return false;
    }

    uint8_t led_idx;
    for (led_idx = 0; led_idx < num_leds; led_idx++) {
        if (scmp(leds[led_idx].name, argv[1])) {
            break;
        }
    }

    if (led_idx == num_leds) {
        printf("invalid led name\n");
        printf("usage: led <color> <on/off/blink>\n");
        return false;
    }

    if (scmp("on", argv[2])) {
        dio_wr(leds[led_idx].io_channel, LED_ON);
        timer_stop(leds[led_idx].timer_id);
    } else if (scmp("off", argv[2])) {
        dio_wr(leds[led_idx].io_channel, LED_OFF);
        timer_stop(leds[led_idx].timer_id);
    } else if (scmp("blink", argv[2])) {
        timer_start(leds[led_idx].timer_id, leds[led_idx].blink_period);
    } else {
        printf("invalid control description\n");
        printf("usage: led <color> <on/off/blink>\n");
        return false;
    }

    return true;
}

static enum timer_action timer_callback(uint8_t led_idx)
{
    leds[led_idx].state = ~leds[led_idx].state;
    dio_wr(leds[led_idx].io_channel, leds[led_idx].state);

    return TIMER_ACTION_REPEAT;
}

void leds_init(struct led_config *cfg, uint8_t len)
{
    leds = malloc(sizeof(struct led_info) * len);
    num_leds = len;

    for (uint8_t i = 0; i < len; i++) {
        leds[i].name = cfg[i].name;
        leds[i].io_channel = cfg[i].io_channel;
        leds[i].blink_period = cfg[i].blink_period;
        leds[i].timer_id = timer_get();
        leds[i].state = LED_OFF;
        timer_set_callback(leds[i].timer_id, timer_callback);
        timer_set_userdata(leds[i].timer_id, i);
    }

    cmd_register("led", led_command);
}
