#include <stdbool.h>
#include <ctype.h>

#include "console.h"
#include "uart.h"
#include "command.h"

static char cmd_buf[CMD_BUF_SIZE];
static uint8_t cmd_buf_idx = 0;

static bool first_run_done = false;

void console_init(void)
{
    cmd_buf_idx = 0;
    first_run_done = false;
}

void console_update(void)
{
    char c;

    if (!first_run_done) {
        uart_puts(PROMPT);
        first_run_done = true;
    }

    while (uart_getc(&c)) {
        switch (c) {
            case '\n':
            case '\r':
                uart_putc('\n');
                cmd_buf[cmd_buf_idx] = '\0';

                // send buffer to command module
                // for processing
                if (cmd_buf_idx > 0) {
                    cmd_execute(cmd_buf);
                }

                cmd_buf_idx = 0;
                uart_puts(PROMPT);
                break;
            // backspace
            case '\b':
            case 0x7f:
                if (cmd_buf_idx > 0) {
                    uart_puts("\b \b");
                    cmd_buf_idx--;
                }
                break;
            // clear screen
            case '\f':
                uart_putc(c);
                cmd_buf_idx = 0;
                uart_puts(PROMPT);
                break;
            default:
                if (isprint(c)) {
                    if (cmd_buf_idx < CMD_BUF_SIZE - 1) {
                            uart_putc(c);
                            cmd_buf[cmd_buf_idx++] = c;
                    } else {
                            // ring bell if buffer is full
                            uart_putc('\a');
                    }
                }
                break;
        }
    }
}
