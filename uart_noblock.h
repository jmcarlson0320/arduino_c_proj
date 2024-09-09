#ifndef UART_NO_BLOCK_H
#define UART_NO_BLOCK_H

#include <stdint.h>
#include <stdbool.h>

#define BAUD 9600

#define RX_BUF_SIZE 32
#define TX_BUF_SIZE 128

void uart_init(void);
void uart_update(void);

bool uart_getc(char *c);
bool uart_putc(char c);
uint8_t uart_puts(char *s);

#endif // UART_NO_BLOCK_H
