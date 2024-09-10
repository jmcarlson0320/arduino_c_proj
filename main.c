#include "uart.h"
#include "console.h"

int main(void)
{
    // call module init api's here
    uart_init();
    console_init();

    // super loop
    while (1) {

        // call module update api's here
        uart_update();
        console_update();

    }
}

