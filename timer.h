#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_TIMERS 8
#define ID_ERROR 255

enum timer_action {
    TIMER_ACTION_NONE,
    TIMER_ACTION_REPEAT
};

typedef enum timer_action (*timer_cb)(uint8_t userdata);

uint32_t get_sys_ticks(void);

void timer_init(void);
void timer_update(void);

uint8_t timer_get(void);
void timer_start(uint8_t id, uint16_t ms);
void timer_stop(uint8_t id);
void timer_release(uint8_t id);
bool timer_is_expired(uint8_t id);
void timer_set_callback(uint8_t id, timer_cb cb);
void timer_set_userdata(uint8_t id, uint8_t userdata);

#endif // TIMER_H
