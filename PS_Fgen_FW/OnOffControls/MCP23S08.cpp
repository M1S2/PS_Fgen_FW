/*
 * MCP23S08.cpp
 * Created: 29.08.2024 18:54:54
 * Author: Markus Scheich
 * Based on: https://github.com/julianschuler/MCP23S08/blob/master/MCP23S08.cpp
 */ 

#include "MCP23S08.h"
#include "../Spi/spi.h"

/*##################################### PUBLIC FUNCTIONS #####################################*/

MCP23S08::MCP23S08() 
{
}

MCP23S08::MCP23S08(uint8_t deviceAddr) 
{
	deviceOpcode |= ((deviceAddr & 0x03) << 1);
}


void MCP23S08::begin()
{
	SELECT_MCP23S08
	// reset all registers to default:
	SPI_SendByte(deviceOpcode);
	SPI_SendByte(MCP23S08_IODIR);	//set address pointer to first register
	SPI_SendByte(0xFF);				// reset first register
	for (uint8_t i = 0; i < MCP23S08_OLAT; i++)
	{
		SPI_SendByte(0x00);			// reset other 10 registers
	}
	DESELECT_MCP23S08
}


bool MCP23S08::digitalReadIO(uint8_t pin)
{
	if (pin > 7)
	{
		return 0;
	}
	return (getInputStates() >> pin) & 1;
}


void MCP23S08::digitalWriteIO(uint8_t pin, bool state)
{
	if (pin > 7)
	{
		return;
	}
	
	setOutputStates((getOutputStates() & ~(1 << pin)) | (state << pin));
}


void MCP23S08::pinModeIO(uint8_t pin, MCP23S08_PinModes_t mode)
{
	if (pin > 7)
	{
		return;
	}
	
	switch (mode)
	{
		case MCP23S08_PINMODE_INPUT:
			setPinModes(getPinModes() & ~(1 << pin));			// set pin to input
			enablePullups(getEnabledPullups() & ~(1 << pin));	// disable pullup for pin
			break;
		case MCP23S08_PINMODE_OUTPUT:
			setPinModes(getPinModes() | (1 << pin));			// set pin to output
			enablePullups(getEnabledPullups() & ~(1 << pin));	// disable pullup for pin
			break;
		case MCP23S08_PINMODE_INPUT_PULLUP:
			setPinModes(getPinModes() & ~(1 << pin));			// set pin to input
			enablePullups(getEnabledPullups() | (1 << pin));	// enable pullup for pin
			break;
	}
}


void MCP23S08::setOutputStates(uint8_t states) 
{
	writeRegister(MCP23S08_OLAT, states);
}


void MCP23S08::setPinModes(uint8_t modes) 
{
	writeRegister(MCP23S08_IODIR, ~(modes));	// inverted to match IDE defaults
}


void MCP23S08::enablePullups(uint8_t enables) 
{
	writeRegister(MCP23S08_GPPU, enables);
}


uint8_t MCP23S08::getInputStates() 
{
	return readRegister(MCP23S08_GPIO);
}


uint8_t MCP23S08::getOutputStates()
{
	return readRegister(MCP23S08_OLAT);
}


uint8_t MCP23S08::getPinModes() 
{
	return ~(readRegister(MCP23S08_IODIR));		// inverted to match IDE defaults
}


uint8_t MCP23S08::getEnabledPullups() 
{
	return readRegister(MCP23S08_GPPU);
}

/************************************** Interrupts *********************************************/

void MCP23S08::setInterruptOnChange(uint8_t mask)
{
	writeRegister(MCP23S08_GPINTEN, mask);
}

void MCP23S08::setInterruptOnChangeDefaultCompare(uint8_t mask)
{
	writeRegister(MCP23S08_DEFVAL, mask);	
}

void MCP23S08::setInterruptControl(uint8_t mask)
{
	writeRegister(MCP23S08_INTCON, mask);		
}

void MCP23S08::setINTPinPushPullActiveState(bool activeLow)
{
	uint8_t ioconVal = readRegister(MCP23S08_IOCON);
	ioconVal &= ~(1 << 2);					// Bit2 = 0: Active driver output (INTPOL bit sets the polarity).
	if(activeLow)
	{
		ioconVal &= ~(1 << 1);				// Bit1 = 0: Active-low. INTPOL: This bit sets the polarity of the INT output pin.		
	}
	else
	{
		ioconVal |= (1 << 1);				// Bit1 = 1: Active-high. INTPOL: This bit sets the polarity of the INT output pin.		
	}
	writeRegister(MCP23S08_IOCON, ioconVal);	
}
	
uint8_t MCP23S08::getInterruptOnChange()
{
	return readRegister(MCP23S08_GPINTEN);
}

uint8_t MCP23S08::getInterruptOnChangeDefaultCompare()
{
	return readRegister(MCP23S08_DEFVAL);
}

uint8_t MCP23S08::getInterruptControl()
{
	return readRegister(MCP23S08_INTCON);	
}

uint8_t MCP23S08::getInterruptFlags()
{
	return readRegister(MCP23S08_INTF);
}

uint8_t MCP23S08::getInterruptCaptures()
{
	return readRegister(MCP23S08_INTCAP);			
}

/*##################################### PRIVATE FUNCTIONS #####################################*/

void MCP23S08::writeRegister(uint8_t address, uint8_t data) 
{
	SELECT_MCP23S08
	SPI_SendByte(deviceOpcode);		// initialize transfer with opcode and R/W-flag cleared
	SPI_SendByte(address);
	SPI_SendByte(data);	
	DESELECT_MCP23S08
}


uint8_t MCP23S08::readRegister(uint8_t address) 
{
	uint8_t data;
	SELECT_MCP23S08
	SPI_SendByte(deviceOpcode | 1);		// initialize transfer with opcode and R/W-flag set
	SPI_SendByte(address);
	
	data = SPI_SendByte(0);			// Read byte by shifting out dummy byte
	DESELECT_MCP23S08
	return data;
}