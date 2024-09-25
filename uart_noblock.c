#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"
#include "buffer.h"

static struct buffer rx_buf;
static struct buffer tx_buf;

static int stdio_putc(char c, FILE *stream)
{
    if (uart_putc(c)) {
        return 0;
    } else {
        return 1;
    }
}
static FILE uart_stream = FDEV_SETUP_STREAM(stdio_putc, NULL, _FDEV_SETUP_WRITE);

static bool uart_is_rx_rdy(void)
{
    return ((UCSR0A & (1 << RXC0))) ? true : false;
}

static bool uart_receive_byte(uint8_t *b)
{
    if (!uart_is_rx_rdy()) {
        return false;
    }

    *b = UDR0;

    return true;
}

static bool uart_is_tx_rdy(void)
{
    return ((UCSR0A & (1 << UDRE0))) ? true : false;
}

static bool uart_transmit_byte(uint8_t data)
{
    if (!uart_is_tx_rdy()) {
        return false;
    }

    UDR0 = data;

    return true;
}

static void poll_rx(void)
{
    uint8_t c;

    if (!uart_is_rx_rdy()) {
        return;
    }

    if (!uart_receive_byte(&c)) {
        return;
    }

    buffer_put(&rx_buf, c);
}

static void poll_tx(void)
{
    uint8_t c;

    if (!uart_is_tx_rdy()) {
        return;
    }

    if (buffer_get(&tx_buf, &c) == BUFFER_ERROR) {
        return;
    }

    uart_transmit_byte(c);
}


void uart_init(void)
{
    uint16_t brr = (F_CPU/16/BAUD) - 1;
    UBRR0L = (brr & 0xff);
    UBRR0H = (brr >> 8);

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    rx_buf = buffer_init(malloc(RX_BUF_SIZE), RX_BUF_SIZE);
    tx_buf = buffer_init(malloc(TX_BUF_SIZE), TX_BUF_SIZE);

    stdout = &uart_stream;
}

void uart_update(void)
{
    poll_rx();
    poll_tx();
}

bool uart_getc(char *c)
{
    return (buffer_get(&rx_buf, (uint8_t *)c) == BUFFER_SUCCESS) ? true : false;
}

bool uart_putc(const char c)
{
    return (buffer_put(&tx_buf, c) == BUFFER_SUCCESS) ? true : false;
}

uint8_t uart_puts(const char *s)
{
    uint8_t count = 0;

    while (*s != '\0') {
        if (uart_putc(*s)) {
            count++;
        } else {
            break;
        }
        s++;
    }

    return count;
}
