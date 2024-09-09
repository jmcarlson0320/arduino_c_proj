#ifndef UART_H
#define UART_H

#define BAUD 9600

void uart_init(void);
void uart_putc(char data);
char uart_getc(void);

#endif
