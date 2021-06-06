/*
 * DDS.cpp
 *
 * Created: 23.12.2020 18:23:15
 *  Author: V17
 */ 

#include "DDS.h"
#include "DAC_MCP492x.h"
#include "../Spi/spi.h"
#include "../Device.h"

DDS_Channel* dds_channel1;
DDS_Channel* dds_channel2;

/* Initialize 8-bit Timer/Counter2 */
void InitDDSTimer()
{	
	TCNT2 = 0x00;										// Reset counter
	OCR2A = ((F_CPU / 128) / DDS_TICK_FREQ);			// Set compare register A (DDS_TICK_FREQ Hz)
	TCCR2A = (1 << WGM21);								// Configure for CTC mode
	TCCR2B = (1 << CS22) | (1 << CS20);					// Prescaler 128
	TIMSK2 = (1 << OCIE2A);								// Enable Output Compare A Match Interrupt
	
	dds_channel1 = (DDS_Channel*)Device.Channels[1];
	dds_channel2 = (DDS_Channel*)Device.Channels[2];
}

void DisableDDS1()
{
	MCP4922_Voltage_Set(0, 'A');
}

void DisableDDS2()
{
	MCP4922_Voltage_Set(0, 'B');
}

void DisableDDSTimer()
{
	TCCR2B = 0;											// No clock source (Timer/Counter stopped).
	DisableDDS1();
	DisableDDS2();
}

//https://www.avrfreaks.net/forum/dds-function-generator-using-atmega328p
ISR(TIMER2_COMPA_vect)
{	
	bool is_mcp4921_selected = IS_MCP4921_SELECTED;
	DESELECT_MCP4921
	
	bool is_lcd_selected = BIT_IS_CLEARED(PORTB, LCD_CS);
	SET_BIT(PORTB, LCD_CS);		//Deselect LCD
	
	SELECT_MCP4922
	
	if(dds_channel1->Enabled.Val)
	{
		uint8_t lut1_index = (dds_channel1->Accumulator >> (DDS_PHASE_ACCU_BITS - DDS_QUANTIZER_BITS));			//index to look-up-table (truncated phase accumulator)
		uint16_t dds1_data = dds_channel1->WaveTable[lut1_index];
	
		uint8_t high_byte = (1 << MCP492X_SHDN) | (1 << MCP492X_BUFFERED) | (1 << MCP492X_GAIN_SELECT_SINGLE);			// Set SHDN bit high for DAC A active operation; Enable buffered inputs for Vref; Select single gain
		high_byte |= ((dds1_data >> 8) & 0x0F);
		uint8_t low_byte = (dds1_data & 0xFF);

		SPI_SPDR = high_byte;
		while (!(SPI_SPSR & (1 << SPI_SPIF)));	// Wait until transmission is complete
		SPI_SPDR = low_byte;
		while (!(SPI_SPSR & (1 << SPI_SPIF)));	// Wait until transmission is complete

		dds_channel1->Accumulator += dds_channel1->Increment;
	}
	if(dds_channel1->Enabled.Val && dds_channel2->Enabled.Val)			// After each write command, the data needs to be shifted into the DAC's input registers by raising the CS pin (The CS pin is then raised, causing the data to be latched into the selected DAC’s input registers.)
	{
		DESELECT_MCP4922
		SELECT_MCP4922
	}
	if(dds_channel2->Enabled.Val)
	{
		uint8_t lut2_index = (dds_channel2->Accumulator >> (DDS_PHASE_ACCU_BITS - DDS_QUANTIZER_BITS));			//index to look-up-table (truncated phase accumulator)
		uint16_t dds2_data = dds_channel2->WaveTable[lut2_index];

		uint8_t high_byte = (1 << MCP492X_SHDN) | (1 << MCP492X_BUFFERED) | (1 << MCP492X_GAIN_SELECT_SINGLE) | (1 << MCP492X_DACB);		// Set SHDN bit high for DAC A active operation; Enable buffered inputs for Vref; Select single gain; Select DAC channel B
		high_byte |= ((dds2_data >> 8) & 0x0F);
		uint8_t low_byte = (dds2_data & 0xFF);

		SPI_SPDR = high_byte;
		while (!(SPI_SPSR & (1 << SPI_SPIF)));	// Wait until transmission is complete
		SPI_SPDR = low_byte;
		while (!(SPI_SPSR & (1 << SPI_SPIF)));	// Wait until transmission is complete

		dds_channel2->Accumulator += dds_channel2->Increment;
	}
	
	DESELECT_MCP4922
	
	if(is_mcp4921_selected) { SELECT_MCP4921 }
	if(is_lcd_selected) { CLEAR_BIT(PORTB, LCD_CS); }
}