#include <avr/io.h>
#include <stdio.h>

#include "uart.h"
#include "console.h"
#include "command.h"
#include "token.h"

bool led_command(uint8_t argc, char *argv[])
{
    if (scmp("on", argv[1])) {
        PORTB |= (1 << PORTB4);
    } else if (scmp("off", argv[1])) {
        PORTB &= ~(1 << PORTB4);
    }

    return true;
}

int main(void)
{
    uart_init();
    console_init();

    cmd_register("led", led_command);

    DDRB |= (1 << DDB4); // output

    while (1) {
        uart_update();
        console_update();
    }
}
