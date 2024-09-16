#include "uart.h"
#include "console.h"
#include "command.h"

// function matching command function prototype in command.h
//
// this is an action performed by a command
// prints a message followed by a list of the arguments
// passed in.
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

    // registers the above function as a command named 'dmy'
    //
    // run the command from the console using:
    //
    // dmy arg1 arg2 arg3 ...
    cmd_register("dmy", dummy_command);

    // super loop
    while (1) {

        // call module update api's here

        // polls the uart registers for rx/tx
        //
        // if rx ready, read in the byte and store in rx_buffer
        //
        // if tx ready and there are bytes to transmit on tx_buffer,
        //      send first byte
        uart_update();

        // gets a character off the uart rx_buffer and processes it
        //      check for non-print characters like backspace, clear screen
        //      maintains line-discipline
        //      stores characters received from uart in a command buffer
        //      upon newline, sends the command buffer to the command module
        //          for processing
        //
        //summary:
        //  gets bytes from uart
        //  sends command string to command module
        console_update();

    }
}

