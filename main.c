#include "uart.h"
#include "console.h"
#include "command.h"

//bool cmd_register(const char *name, cmd_func f)

bool dummy_command(uint8_t argc, char *argv[])
{
    uart_puts("running dummy command\n");
    uart_puts("arguments: ");
    for (uint8_t i = 0; i < argc; i++) {
        uart_puts(argv[i]);
        if (i < (argc - 1)) {
            uart_puts(", ");
        }
    }
    uart_putc('\n');
    return true;
}
    

int main(void)
{
    // call module init api's here
    uart_init();
    console_init();

    cmd_register("dm_cmd", dummy_command);

    // super loop
    while (1) {

        // call module update api's here
        uart_update();
        console_update();

    }
}

