#include "uart_noblock.h"

int main(void)
{
    char c;

    // call module init api's here
    uart_init();

    uart_puts("joshua carlson\n");

    // super loop
    while (1) {

        // call module update api's here
        uart_update();

        // echo receive bytes
        if (uart_getc(&c)) {
            uart_putc(c);
        }
    }
}

