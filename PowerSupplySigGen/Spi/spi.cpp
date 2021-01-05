/*
 * spi.cpp
 *
 * Created: 14.08.2020 19:24:05
 *  Author: V17
 * Source: https://github.com/0xD34D/ATmega/blob/master/spi.c
 */ 

#include "spi.h"

// Initialize the SPI as master
void SPI_Init()
{
	// set up the SPI module: SPI enabled, MSB first, master mode, clock polarity and phase = 0, F_osc/128 (/16)
	SPI_SPCR = ( 1 << SPI_SPE ) | ( 1 << SPI_MSTR ); //| ( 1 << SPI_SPR0 ) | ( 1 << SPI_SPR1 );
	SPI_SPSR = 1;     // set double SPI speed for F_osc/2
}

// Transfer a byte of data
uint8_t SPI_SendByte( uint8_t data )
{		
	// Start transmission
	SPI_SPDR = data;

	// Wait for the transmission to complete
	spi_wait();

	// return the byte received from the slave
	return SPI_SPDR;
}


// Transfer a byte of data
uint8_t SPI_ReadByte( void )
{
	// Start transmission
	SPI_SPDR = 0xFF;

	// Wait for the transmission to complete
	spi_wait();

	// return the byte received from the slave
	return SPI_SPDR;
}