#include <avr/io.h>

#include "uart_basic.h"

void uart_init(void)
{
    uint16_t brr = (F_CPU/16/BAUD) - 1;
    UBRR0L = (brr & 0xff);
    UBRR0H = (brr >> 8);

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void uart_putc(char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

char uart_getc(void)
{
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}
