/*
 * uart.c
 *
 * Created: 14/12/2025 08:24:47
 *  Author: Francisco Menezes
 */ 
#define F_CPU 1000000UL


#include "uart.h"
#include <avr/io.h>



void uart_init(uint32_t baud)
{
	// Usando U2X para reduzir erro em 1MHz
	UCSR0A = (1<<U2X0);

	// UBRR = F_CPU/(8*baud) - 1
	uint16_t ubrr = (uint16_t)((F_CPU / (8UL * baud)) - 1);

	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)(ubrr & 0xFF);

	UCSR0B = (1<<TXEN0);                 // só TX
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);  // 8N1
}

void uart_putc(char c)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
}

void uart_puts(const char *s)
{
	while(*s) uart_putc(*s++);
}

static void uart_putnum(int32_t v)
{
	char buf[16];
	uint8_t i = 0;

	if (v == 0) { uart_putc('0'); return; }

	if (v < 0) { uart_putc('-'); v = -v; }

	while(v > 0 && i < sizeof(buf)) {
		buf[i++] = '0' + (v % 10);
		v /= 10;
	}
	while(i) uart_putc(buf[--i]);
}

void uart_print_int(const char *label, int32_t v)
{
	uart_puts(label);
	uart_putnum(v);
	uart_puts("\r\n");
}
