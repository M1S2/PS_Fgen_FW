/*
 * DDS.cpp
 * Created: 23.12.2020 18:23:15
 * Author: Markus Scheich
 */ 

#include "DDS.h"
#include "DAC_MCP492x.h"
#include "../Spi/spi.h"
#include "../Device.h"

#ifdef DDS_SUBSYSTEM_ENABLED

volatile uint32_t dds_channel1_accumulator;				// This variables aren't part of the DDS_Channel class to speed up the DDS ISR (no dereferencing neccessary)
volatile uint32_t dds_channel2_accumulator;
volatile uint32_t dds_channel1_increment;
volatile uint32_t dds_channel2_increment;
volatile uint16_t dds_channel1_waveTable[(1 << DDS_QUANTIZER_BITS)];		// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
volatile uint16_t dds_channel2_waveTable[(1 << DDS_QUANTIZER_BITS)];		// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
volatile bool dds_channel1_enabled;
volatile bool dds_channel2_enabled;

/* Initialize 8-bit Timer/Counter2 */
void InitDDSTimer()
{
	TCNT2 = 0x00;										// Reset counter
	OCR2A = ((F_CPU / 1024) / DDS_TICK_FREQ);			// Set compare register A (DDS_TICK_FREQ Hz)
	TCCR2A = (1 << WGM21);								// Configure for CTC mode
	TCCR2B = 0;											// No clock source (Timer/Counter stopped).
}

void StartDDSTimer()
{
	dds_channel1_accumulator = 0;
	dds_channel2_accumulator = 0;

	TCNT2 = 0x00;										// Reset counter
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);	// Prescaler 1024
	TIMSK2 = (1 << OCIE2A);								// Enable Output Compare A Match Interrupt
}

void DisableDDSTimer()
{
	TCCR2B = 0;											// No clock source (Timer/Counter stopped).
}

void DisableDDS1()
{
	MCP4922_Voltage_Set(0, 'A');
}

void DisableDDS2()
{
	MCP4922_Voltage_Set(0, 'B');
}

//https://www.avrfreaks.net/forum/dds-function-generator-using-atmega328p
ISR(TIMER2_COMPA_vect)
{
	cli();
	
	SPI_DESELECT_ALL
	SPI_SELECT_DDS
	
	if(dds_channel1_enabled)
	{
		uint8_t lut1_index = (dds_channel1_accumulator >> (DDS_PHASE_ACCU_BITS - DDS_QUANTIZER_BITS));			//index to look-up-table (truncated phase accumulator)
		uint16_t dds1_data = dds_channel1_waveTable[lut1_index];
	
		uint8_t high_byte = (1 << MCP492X_SHDN) | (1 << MCP492X_BUFFERED) | (1 << MCP492X_GAIN_SELECT_SINGLE);			// Set SHDN bit high for DAC A active operation; Enable buffered inputs for Vref; Select single gain
		high_byte |= ((dds1_data >> 8) & 0x0F);
		
		SPDR = high_byte;
		uint8_t low_byte = (dds1_data & 0xFF);
		while (!(SPSR & (1 << SPIF)));	// Wait until transmission is complete
		SPDR = low_byte;
		dds_channel1_accumulator += dds_channel1_increment;
		while (!(SPSR & (1 << SPIF)));	// Wait until transmission is complete
	}
	if(dds_channel1_enabled && dds_channel2_enabled)					// After each write command, the data needs to be shifted into the DAC's input registers by raising the CS pin (The CS pin is then raised, causing the data to be latched into the selected DAC's input registers.)
	{
		SPI_DESELECT_DDS
		SPI_SELECT_DDS
	}
	if(dds_channel2_enabled)
	{
		uint8_t lut2_index = (dds_channel2_accumulator >> (DDS_PHASE_ACCU_BITS - DDS_QUANTIZER_BITS));			//index to look-up-table (truncated phase accumulator)
		uint16_t dds2_data = dds_channel2_waveTable[lut2_index];

		uint8_t high_byte = (1 << MCP492X_SHDN) | (1 << MCP492X_BUFFERED) | (1 << MCP492X_GAIN_SELECT_SINGLE) | (1 << MCP492X_DACB);		// Set SHDN bit high for DAC A active operation; Enable buffered inputs for Vref; Select single gain; Select DAC channel B
		high_byte |= ((dds2_data >> 8) & 0x0F);

		SPDR = high_byte;
		uint8_t low_byte = (dds2_data & 0xFF);
		while (!(SPSR & (1 << SPIF)));	// Wait until transmission is complete
		SPDR = low_byte;
		dds_channel2_accumulator += dds_channel2_increment;
		while (!(SPSR & (1 << SPIF)));	// Wait until transmission is complete
	}

	SPI_DESELECT_DDS
	SPI_SELECT_TFT

	sei();
}

#endif /* DDS_SUBSYSTEM_ENABLED */