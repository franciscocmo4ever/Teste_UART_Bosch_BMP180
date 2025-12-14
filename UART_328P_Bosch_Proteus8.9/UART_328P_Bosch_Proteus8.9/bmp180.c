/*
 * bmp180.h
 *
 * Created: 14/12/2025 08:25:43
 *  Author: Francisco Menezes
 */ 
#define F_CPU 1000000UL


#include "bmp180.h"
#include "twi_master.h"
#include <util/delay.h>

static int16_t s16(uint8_t msb, uint8_t lsb){ return (int16_t)((msb<<8) | lsb); }
static uint16_t u16(uint8_t msb, uint8_t lsb){ return (uint16_t)((msb<<8) | lsb); }

static uint8_t bmp180_write8(uint8_t reg, uint8_t val)
{
	if (twi_start((BMP180_ADDR<<1) | 0)) return 1;
	if (twi_write(reg)) { twi_stop(); return 2; }
	if (twi_write(val)) { twi_stop(); return 3; }
	twi_stop();
	return 0;
}

static uint8_t bmp180_read_bytes(uint8_t reg, uint8_t *buf, uint8_t n)
{
	if (twi_start((BMP180_ADDR<<1) | 0)) return 1;
	if (twi_write(reg)) { twi_stop(); return 2; }

	if (twi_start((BMP180_ADDR<<1) | 1)) { twi_stop(); return 3; }

	for (uint8_t i=0; i<n-1; i++) buf[i] = twi_read_ack();
	buf[n-1] = twi_read_nack();
	twi_stop();
	return 0;
}

uint8_t bmp180_read_calib(bmp180_calib_t *c)
{
	uint8_t buf[22];
	uint8_t err = bmp180_read_bytes(0xAA, buf, 22);
	if (err) return err;

	c->AC1 = s16(buf[0],  buf[1]);
	c->AC2 = s16(buf[2],  buf[3]);
	c->AC3 = s16(buf[4],  buf[5]);
	c->AC4 = u16(buf[6],  buf[7]);
	c->AC5 = u16(buf[8],  buf[9]);
	c->AC6 = u16(buf[10], buf[11]);
	c->B1  = s16(buf[12], buf[13]);
	c->B2  = s16(buf[14], buf[15]);
	c->MB  = s16(buf[16], buf[17]);
	c->MC  = s16(buf[18], buf[19]);
	c->MD  = s16(buf[20], buf[21]);

	return 0;
}

uint8_t bmp180_read_ut(int32_t *ut)
{
	uint8_t err = bmp180_write8(0xF4, 0x2E);  // start temp
	if (err) return err;

	_delay_ms(5); // 4.5ms -> 5ms seguro

	uint8_t b[2];
	err = bmp180_read_bytes(0xF6, b, 2);
	if (err) return err;

	*ut = (int32_t)((b[0] << 8) | b[1]);
	return 0;
}

uint8_t bmp180_read_up(uint8_t oss, int32_t *up)
{
	uint8_t cmd = (uint8_t)(0x34 + (oss << 6));
	uint8_t err = bmp180_write8(0xF4, cmd);   // start pressure
	if (err) return err;

	// tempo depende do oss (datasheet)
	if      (oss == 0) _delay_ms(5);
	else if (oss == 1) _delay_ms(8);
	else if (oss == 2) _delay_ms(14);
	else               _delay_ms(26);

	uint8_t b[3];
	err = bmp180_read_bytes(0xF6, b, 3);
	if (err) return err;

	int32_t raw = ((int32_t)b[0] << 16) | ((int32_t)b[1] << 8) | b[2];
	*up = raw >> (8 - oss);
	return 0;
}
