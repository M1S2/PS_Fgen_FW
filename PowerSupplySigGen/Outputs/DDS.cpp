/*
 * DDS.cpp
 *
 * Created: 23.12.2020 18:23:15
 *  Author: V17
 */ 

#include "DDS.h"
#include "../Spi/spi.h"
#include "DAC_MCP492x.h"

/* Initialize 8-bit Timer/Counter2 */
void InitDDSTimer()
{	
	TCNT2 = 0x00;										// Reset counter
	OCR2A = ((F_CPU / 1024) / DDS_TICK_FREQ);			// Set compare register A (DDS_TICK_FREQ Hz)
	TCCR2A = (1 << WGM21);								// Configure for CTC mode
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);	// Prescaler 1024						//(1 << CS20);				// No Prescaler
	TIMSK2 = (1 << OCIE2A);								// Enable Output Compare A Match Interrupt
	
	DDS_Channel1.SetSignalForm(SINE);
	DDS_Channel1.SetFrequency(1000);
}

//https://www.avrfreaks.net/forum/dds-function-generator-using-atmega328p
ISR(TIMER2_COMPA_vect)
{	
	uint8_t lut1_index = (DDS_Channel1.Accumulator >> (DDS_PHASE_ACCU_BITS - DDS_QUANTIZER_BITS));			//index to look-up-table (truncated phase accumulator)
	uint16_t dds1_data = pgm_read_word(&DDS_Channel1.OriginalWaveTable[lut1_index]);
	
	bool is_mcp4921_selected = IS_MCP4921_SELECTED;
	DESELECT_MCP4921
	
	bool is_lcd_selected = BIT_IS_CLEARED(PORTB, LCD_CS);
	SET_BIT(PORTB, LCD_CS);		//Deselect LCD
	
	SELECT_MCP4922
	uint8_t high_byte = (1 << MCP492X_SHDN) | (1 << MCP492X_BUFFERED) | (1 << MCP492X_GAIN_SELECT_SINGLE);			// Set SHDN bit high for DAC A active operation; Enable buffered inputs for Vref; Select single gain
	high_byte |= ((dds1_data >> 8) & 0x0F);
	uint8_t low_byte = (dds1_data & 0xFF);
	
	SPI_SPDR = high_byte;
	while (!(SPI_SPSR & (1 << SPI_SPIF)));	// Wait until transmission is complete
	SPI_SPDR = low_byte;
	while (!(SPI_SPSR & (1 << SPI_SPIF)));	// Wait until transmission is complete

	DESELECT_MCP4922
	
	if(is_mcp4921_selected) { SELECT_MCP4921 }
	if(is_lcd_selected) { CLEAR_BIT(PORTB, LCD_CS); }	
	
	DDS_Channel1.Accumulator += DDS_Channel1.Increment;
}