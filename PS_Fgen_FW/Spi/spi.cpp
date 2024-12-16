/*
 * spi.cpp
 * Created: 14.08.2020 19:24:05
 */ 

#include "spi.h"

void SPI_SelectDevice(SpiDevices_t spiDevice)
{
	// Set the bits in the bitmask on PortA and PortB to high (chip select disabled)
	PORTA |= BITMASK_CS_PORTA;
	PORTB |= BITMASK_CS_PORTB;

	switch (spiDevice)
	{
		case SPI_DEV_TOUCH: SPI_SELECT_TOUCH break;
		case SPI_DEV_TFT: SPI_SELECT_LCD break;
		case SPI_DEV_PS: SPI_SELECT_PS break;
		case SPI_DEV_DDS: SPI_SELECT_DDS break;
		case SPI_DEV_IO_EXP: SPI_SELECT_IO_EXP break;
		case SPI_DEV_NONE: break;
		default: break;
	}
}

void SPI_DeselectAll()
{
	SPI_SelectDevice(SPI_DEV_NONE);
}