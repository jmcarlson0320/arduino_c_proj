#include <avr/interrupt.h>
#include <stdio.h>

#include "timer.h"

enum timer_state {
    TIMER_UNUSED,
    TIMER_RUNNING,
    TIMER_STOPPED,
    TIMER_EXPIRED
};

struct timer {
    uint32_t start;
    uint32_t duration;
    enum timer_state state;
    timer_cb callback;
    uint8_t userdata;
};

static struct timer timers[MAX_TIMERS];

static volatile uint32_t ticks = 0;

ISR(TIMER0_COMPA_vect)
{
    ticks++;
}

uint32_t get_sys_ticks(void)
{
    return ticks;
}

void timer_init(void)
{
    for (uint8_t i = 0; i < MAX_TIMERS; i++) {
        timers[i].state = TIMER_UNUSED;
        timers[i].start = 0;
        timers[i].duration = 0;
        timers[i].callback = NULL;
    }

    ticks = 0;

    // set timer mode to CTC: clear timer on compare match
    // OCR0A determines the max value of the counter, and the
    // period of the roll-overs
    TCCR0A = (1 << WGM01);

    // sets timer to use prescaled clock
    // clk / 64
    // timer frequency = 16mhz / 64 = 250khz
    TCCR0B = TCCR0B | (1 << CS01) | (1 << CS00);

    // set timer compare value to 249
    // timer ticks 250 times before rolling over
    // roll-over frequency = 250khz / 250 = 1khz
    OCR0A = 249;

    // enable timer interrupts for compare/match A
    TIMSK0 = (1 << OCIE0A);

    sei();
}

void timer_update(void)
{
    for (uint8_t i = 0; i < MAX_TIMERS; i++) {
        if (timers[i].state != TIMER_RUNNING) {
            continue;
        }
        if ((ticks - timers[i].start) < timers[i].duration) {
            continue;
        }

        if (timers[i].callback) {
            enum timer_action action = (*(timers[i].callback))(timers[i].userdata);
            if (action == TIMER_ACTION_REPEAT) {
                timers[i].start = ticks;
                timers[i].state = TIMER_RUNNING;
            } else if (action == TIMER_ACTION_NONE) {
                timers[i].state = TIMER_EXPIRED;
            }
        } else {
            timers[i].state = TIMER_EXPIRED;
        }
    }
}

uint8_t timer_get(void)
{
    uint8_t i;

    for (i = 0; i < MAX_TIMERS; i++) {
        if (timers[i].state == TIMER_UNUSED) {
            timers[i].state = TIMER_STOPPED;
            timers[i].callback = NULL;
            return i;
        }
    }

    return ID_ERROR;
}

void timer_start(uint8_t id, uint16_t ms)
{
    if (id < 0 || MAX_TIMERS <= id) {
        return;
    }

    if (timers[id].state == TIMER_UNUSED) {
        return;
    }

    timers[id].start = ticks;
    timers[id].duration = ms;
    timers[id].state = TIMER_RUNNING;
}

void timer_stop(uint8_t id)
{
    if (id < 0 || MAX_TIMERS <= id) {
        return;
    }

    timers[id].state = TIMER_STOPPED;
}

void timer_release(uint8_t id)
{
    if (id < 0 || MAX_TIMERS <= id) {
        return;
    }

    timers[id].state = TIMER_UNUSED;
}

bool timer_is_expired(uint8_t id)
{
    if (timers[id].state == TIMER_EXPIRED) {
        timers[id].state = TIMER_STOPPED;
        return true;
    }

    return false;
}

void timer_set_callback(uint8_t id, timer_cb cb)
{
    if (id < 0 || MAX_TIMERS <= id) {
        return;
    }
    
    timers[id].callback = cb;
}

void timer_set_userdata(uint8_t id, uint8_t userdata)
{
    if (id < 0 || MAX_TIMERS <= id) {
        return;
    }

    timers[id].userdata = userdata;
}
