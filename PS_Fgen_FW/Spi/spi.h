/**
 * @file	spi.h
 * @date	14.08.2020 19:24:17
 * @author	Markus Scheich
 * @brief	Containing defines and functions for basic SPI initialization and handling.
 * @see		https://github.com/0xD34D/ATmega/blob/master/include/spi.h
 */ 

#ifndef SPI_H_
#define SPI_H_

#include "../Pins/Pins.h"

#define spi_wait()	while (!(SPSR & (1 << SPIF)));		/**< Loop until any current SPI transmissions have completed */

/**
 * Initialize the SPI subsystem as master.
 */
void SPI_Init();

/**
 * Transfer a byte of data to the slave.
 * @param data Data byte to transfer via SPI.
 * @return Byte received from the slave.
 */
uint8_t SPI_SendByte(uint8_t data);

/**
 * Read a byte of data from the slave by sending a dummy byte.
 * @return Byte received form the slave.
 */
uint8_t SPI_ReadByte();

#endif /* SPI_H_ */