#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_init(void);

uint8_t timer_get(uint16_t ms);
void timer_start(uint8_t id, uint16_t ms);
void timer_release(uint8_t id);

#endif // TIMER_H
