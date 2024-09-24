#ifndef LED_H
#define LED_H

#include "dio.h"

struct led_config {
    char *name;
    uint8_t io_channel;
    uint16_t blink_period;
};

void leds_init(struct led_config *cfg, uint8_t len);

#endif // LED_H
