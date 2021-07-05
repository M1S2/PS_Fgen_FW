/*
 * spi.cpp
 * Created: 14.08.2020 19:24:05
 * Author: Markus Scheich
 * Source: https://github.com/0xD34D/ATmega/blob/master/spi.c
 */ 

#include "spi.h"

void SPI_Init()
{
	SPCR = (1 << SPE) | (1 << MSTR);		// SPI enabled, MSB first (DORD=0), master mode, F_osc/2
	SPSR = 1;								// set double SPI speed for F_osc/2
}

uint8_t SPI_SendByte(uint8_t data)
{		
	SPDR = data;		// Start transmission
	spi_wait();			// Wait for the transmission to complete
	return SPDR;		// return the byte received from the slave
}

uint8_t SPI_ReadByte()
{
	SPDR = 0xFF;		// Start transmission
	spi_wait();			// Wait for the transmission to complete
	return SPDR;		// return the byte received from the slave
}