#include <stdio.h>

#include "uart.h"
#include "console.h"
#include "command.h"

bool dummy_command(uint8_t argc, char *argv[])
{
    printf("running the command: %s\n", argv[0]);
    for (uint8_t i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    return true;
}

int main(void)
{
    uart_init();
    console_init();

    cmd_register("cmd_1", dummy_command);

    while (1) {
        uart_update();
        console_update();
    }
}
