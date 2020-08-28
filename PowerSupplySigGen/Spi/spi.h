/*
 * spi.h
 *
 * Created: 14.08.2020 19:24:17
 *  Author: V17
 * Source: https://github.com/0xD34D/ATmega/blob/master/include/spi.h
 */ 

#ifndef SPI_H_
#define SPI_H_

#include "../Pins/Pins.h"

#define SPI_SPCR	SPCR
#define SPI_SPDR	SPDR
#define SPI_SPSR	SPSR
#define SPI_SPIF	SPIF
#define SPI_SPE		SPE
#define SPI_MSTR	MSTR
#define SPI_SPR0	SPR0
#define SPI_SPR1	SPR1

// Loop until any current SPI transmissions have completed
#define spi_wait()	while (!(SPI_SPSR & (1 << SPI_SPIF)));

// Initialize the SPI subsystem
void SPI_Init();

// Transfer a byte of data
uint8_t SPI_SendByte( uint8_t data );

// Read a byte of data
uint8_t SPI_ReadByte( void );

#endif /* SPI_H_ */