#include "uart.h"

#define UART0_BASE 0x10000000UL
#define UART_THR 0
#define UART_LSR 5
#define UART_LSR_THRE 0x20

static volatile uint8_t *const uart0 = (volatile uint8_t *)UART0_BASE;

void uart_putc(char c)
{
    while ((uart0[UART_LSR] & UART_LSR_THRE) == 0) {
    }

    uart0[UART_THR] = (uint8_t)c;
}

void uart_puts(const char *s)
{
    while (*s != '\0') {
        if (*s == '\n') {
            uart_putc('\r');
        }

        uart_putc(*s++);
    }
}

void uart_put_dec(int64_t v)
{
    char buf[21];
    unsigned int i = 0;
    uint64_t u = (uint64_t)v;

    if (v < 0) {
        uart_putc('-');
        u = (uint64_t)(-(v + 1)) + 1;
    }

    do {
        buf[i++] = (char)('0' + u % 10);
        u /= 10;
    } while (u != 0);

    while (i > 0) {
        uart_putc(buf[--i]);
    }
}

void uart_put_hex(uint64_t v)
{
    static const char digits[] = "0123456789abcdef";
    int started = 0;

    uart_puts("0x");

    for (int shift = 60; shift >= 0; shift -= 4) {
        unsigned int d = (unsigned int)((v >> shift) & 0xf);

        if (d != 0 || started || shift == 0) {
            uart_putc(digits[d]);
            started = 1;
        }
    }
}
