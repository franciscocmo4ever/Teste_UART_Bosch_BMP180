/*
 * GccApplication1.c
 *
 * Created: 14/12/2025 08:19:32
 * Author : Francisco Menezes
 */ 
#define F_CPU 1000000UL

#include <util/delay.h>
#include "twi_master.h"
#include "uart.h"
#include "bmp180.h"

int main(void)
{
	twi_init();
	uart_init(9600);

	uart_puts("\r\n=== BMP180 DUMP ===\r\n");

	bmp180_calib_t cal;
	uint8_t err = bmp180_read_calib(&cal);
	if (err) { uart_print_int("ERRO calib code=", err); while(1); }

	uart_print_int("AC1=", cal.AC1);
	uart_print_int("AC2=", cal.AC2);
	uart_print_int("AC3=", cal.AC3);
	uart_print_int("AC4=", cal.AC4);
	uart_print_int("AC5=", cal.AC5);
	uart_print_int("AC6=", cal.AC6);
	uart_print_int("B1 =", cal.B1);
	uart_print_int("B2 =", cal.B2);
	uart_print_int("MB =", cal.MB);
	uart_print_int("MC =", cal.MC);
	uart_print_int("MD =", cal.MD);

	int32_t UT, UP;
	uint8_t oss = 0; // 0..3

	err = bmp180_read_ut(&UT);
	if (err) { uart_print_int("ERRO UT code=", err); while(1); }

	err = bmp180_read_up(oss, &UP);
	if (err) { uart_print_int("ERRO UP code=", err); while(1); }

	uart_print_int("UT=", UT);
	uart_print_int("UP=", UP);
	uart_print_int("oss=", oss);

	uart_puts("=== FIM ===\r\n");

	while(1) { _delay_ms(1000); }
}
