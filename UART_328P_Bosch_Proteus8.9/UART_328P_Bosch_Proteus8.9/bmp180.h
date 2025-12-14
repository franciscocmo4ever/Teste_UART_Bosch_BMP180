#ifndef BMP180_H_
#define BMP180_H_

#include <stdint.h>

#define BMP180_ADDR 0x77

typedef struct {
	int16_t  AC1, AC2, AC3, B1, B2, MB, MC, MD;
	uint16_t AC4, AC5, AC6;
} bmp180_calib_t;

uint8_t bmp180_read_calib(bmp180_calib_t *c);

// NOVO:
uint8_t bmp180_read_ut(int32_t *ut);
uint8_t bmp180_read_up(uint8_t oss, int32_t *up);

#endif
