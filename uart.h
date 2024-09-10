#ifndef UART_NO_BLOCK_H
#define UART_NO_BLOCK_H

#include <stdint.h>
#include <stdbool.h>

#define BAUD 9600

#define RX_BUF_SIZE 32
#define TX_BUF_SIZE 64

void uart_init(void);
void uart_update(void);

bool uart_getc(char *c);
bool uart_putc(const char c);
uint8_t uart_puts(const char *s);

#endif // UART_NO_BLOCK_H
