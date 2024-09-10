#include <stdbool.h>
#include <ctype.h>

#include "console.h"
#include "uart.h"

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

        // handle end-of-line
        if (c == '\n' || c == '\r') {
            uart_putc('\n');
            cmd_buf[cmd_buf_idx] = '\0';

            // dummy command code
            // simply echoes command
            // here you would dispatch to a command handler
            // ex: bool execute(char *cmd, uint8_t len);
            // execute(cmd_buf, cmd_buf_idx);
            if (uart_puts(cmd_buf) > 1) {
                uart_putc('\n');
            }

            // reset command buffer and prompt
            cmd_buf_idx = 0;
            uart_puts(PROMPT);
            continue;
        }

        // handle backspace
        if (c == '\b' || c == 0x7f) {
            if (cmd_buf_idx > 0) {
                uart_puts("\b \b");
                cmd_buf_idx--;
            }
            continue;
        }

        // handle clear-screen
        if (c == '\f') {
            uart_putc(c);
            cmd_buf_idx = 0;
            uart_puts(PROMPT);
            continue;
        }

        // normal character
        // accumulate in cmd_buffer
        if (isprint(c)) {
            if (cmd_buf_idx < CMD_BUF_SIZE - 1) {
                    uart_putc(c);
                    cmd_buf[cmd_buf_idx++] = c;
            } else {
                    // ring bell if buffer is full
                    uart_putc('\a');
            }
            continue;
        }
    }
}
