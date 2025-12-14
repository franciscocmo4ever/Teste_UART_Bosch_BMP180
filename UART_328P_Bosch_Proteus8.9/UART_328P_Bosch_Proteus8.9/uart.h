/*
 * uart.h
 *
 * Created: 14/12/2025 08:24:08
 *  Author: Francisco Menezes
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void uart_init(uint32_t baud);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_print_int(const char *label, int32_t v);

#endif
