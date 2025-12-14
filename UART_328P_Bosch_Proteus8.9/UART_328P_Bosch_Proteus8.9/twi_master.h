/*
 * twi_master.h
 *
 * Created: 14/12/2025 08:22:47
 *  Author: Francisco Menezes
 */ 

#ifndef TWI_MASTER_H_
#define TWI_MASTER_H_

#include <stdint.h>

void twi_init(void);
uint8_t twi_start(uint8_t address);
void twi_stop(void);
uint8_t twi_write(uint8_t data);
uint8_t twi_read_ack(void);
uint8_t twi_read_nack(void);

#endif
