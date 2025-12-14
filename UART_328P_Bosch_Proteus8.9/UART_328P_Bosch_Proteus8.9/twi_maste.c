/*
 * twi_maste.c
 *
 * Created: 14/12/2025 08:23:24
 *  Author: franc
 */ 
#define F_CPU 1000000UL

#include "twi_master.h"
#include <avr/io.h>





void twi_init(void)
{
	// Com F_CPU=1MHz, o máximo real fica ~62.5kHz (TWBR=0).
	// Aqui deixo ~50kHz para ficar bem estável:
	// SCL = F_CPU / (16 + 2*TWBR)  (prescaler=1)
	TWSR = 0x00;   // prescaler = 1
	TWBR = 2;      // ~50kHz
	TWCR = (1<<TWEN);
}

uint8_t twi_start(uint8_t address)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));

	// envia SLA+W ou SLA+R
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));

	// se não tiver ACK, retorna erro (1)
	uint8_t status = TWSR & 0xF8;
	if (status != 0x18 && status != 0x40) return 1; // SLA+W ACK (0x18) ou SLA+R ACK (0x40)
	return 0;
}

void twi_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

uint8_t twi_write(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));

	uint8_t status = TWSR & 0xF8;
	if (status != 0x28) return 1; // DATA ACK = 0x28
	return 0;
}

uint8_t twi_read_ack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));
	return TWDR;
}

uint8_t twi_read_nack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	return TWDR;
}
