#include <avr/io.h>
#include <stdbool.h>

#include "uart.h"

static uint8_t rx_buf[RX_BUF_SIZE];
static uint16_t rx_put_idx = 0;
static uint16_t rx_get_idx = 0;
static uint8_t rx_buf_count = 0;

static uint8_t tx_buf[TX_BUF_SIZE];
static uint16_t tx_put_idx = 0;
static uint16_t tx_get_idx = 0;
static uint8_t tx_buf_count = 0;

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

    if (rx_buf_count == RX_BUF_SIZE) {
        return;
    }

    rx_buf[rx_put_idx++] = c;
    rx_put_idx %= RX_BUF_SIZE;

    rx_buf_count++;
}

static void poll_tx(void)
{
    uint8_t c;

    if (tx_buf_count == 0) {
        return;
    }

    if (!uart_is_tx_rdy()) {
        return;
    }

    c = tx_buf[tx_get_idx++];
    tx_get_idx %= TX_BUF_SIZE;

    uart_transmit_byte(c);

    tx_buf_count--;
}


/*
 * Public API
 */
void uart_init(void)
{
    uint16_t brr = (F_CPU/16/BAUD) - 1;
    UBRR0L = (brr & 0xff);
    UBRR0H = (brr >> 8);

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void uart_update(void)
{
    poll_rx();
    poll_tx();
}

bool uart_getc(char *c)
{
    if (rx_buf_count == 0) {
        return false;
    }

    *c = rx_buf[rx_get_idx++];
    rx_get_idx %= RX_BUF_SIZE;

    rx_buf_count--;

    return true;
}

bool uart_putc(const char c)
{
    if (tx_buf_count == TX_BUF_SIZE) {
        return false;
    }

    tx_buf[tx_put_idx++] = c;
    tx_put_idx %= TX_BUF_SIZE;

    tx_buf_count++;

    return true;
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
