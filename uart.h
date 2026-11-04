#ifndef BVS1_UART_H
#define BVS1_UART_H

#include <stdint.h>

/* Provided scaffold: polled output on the QEMU virt 16550 UART. */

void uart_putc(char c);
void uart_puts(const char *s);
void uart_put_dec(int64_t v);
void uart_put_hex(uint64_t v);

#endif
