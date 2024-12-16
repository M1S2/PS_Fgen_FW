/**
 * @file	spi.h
 * @date	14.08.2020 19:24:17
 * @brief	Containing defines and functions for basic SPI initialization and handling.
 */ 

#ifndef SPI_H_
#define SPI_H_

#include "../Pins/Pins.h"
#include <SPI.h>

/**
 * Enumeration with all SPI devices.
 */
typedef enum SpiDevices
{
	SPI_DEV_NONE,		/**< No SPI device */
	SPI_DEV_TOUCH,		/**< Touch controller XPT2046. */
	SPI_DEV_TFT,		/**< TFT controller ILI9341 */
	SPI_DEV_PS,			/**< Power supply channel DAC MCP4921 */
	SPI_DEV_DDS,		/**< DDS channel DAC MCP4922 */
	SPI_DEV_IO_EXP		/**< IO expander MCP23S08 */
}SpiDevices_t; 

#define SPI_DEFAULT_SETTINGS	SPISettings(8000000, MSBFIRST, SPI_MODE0)

/**
 * Initialize the SPI subsystem as master.
 */
inline void SPI_Init()
{
	SPI.begin();
}

/**
 * Deselect all SPI devices (pull the CS lines high) and select the given one (pull the CS line low).
 * @param spiDevice Requested SPI device which CS line is pulled low.
 */
void SPI_SelectDevice(SpiDevices_t spiDevice);

/**
 * Deselect all SPI devices (pull the CS lines high).
 */
void SPI_DeselectAll();

/**
 * Transfer a byte of data to the slave.
 * @param data Data byte to transfer via SPI.
 * @return Byte received from the slave.
 */
inline uint8_t SPI_SendByte(uint8_t data)
{
	SPI.beginTransaction(SPI_DEFAULT_SETTINGS);
	uint8_t dataReceived = SPI.transfer(data);
	SPI.endTransaction();
	return dataReceived;
}

/**
 * Read a byte of data from the slave by sending a dummy byte (0xFF).
 * @return Byte received form the slave.
 */
inline uint8_t SPI_ReadByte()
{
	SPI.beginTransaction(SPI_DEFAULT_SETTINGS);
	uint8_t dataReceived = SPI.transfer(0xFF);
	SPI.endTransaction();
	return dataReceived;
}

#endif /* SPI_H_ */