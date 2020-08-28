/*
 * Pins.cpp
 *
 * Created: 23.08.2020 21:40:04
 *  Author: V17
 */ 

#include "Pins.h"

// Initialize the Pins
void Pins_Init()
{
	// make all ADC pins inputs
	CLEAR_BIT(DDRA, ADC_DVM2);
	CLEAR_BIT(DDRA, ADC_DVM1);
	CLEAR_BIT(DDRA, ADC_3V3);
	CLEAR_BIT(DDRA, ADC_5V);
	CLEAR_BIT(DDRA, ADC_12V);
	CLEAR_BIT(DDRA, ADC_12V_NEG);
	CLEAR_BIT(DDRA, ADC_PS_VOLT);
	CLEAR_BIT(DDRA, ADC_PS_CUR);
	
	// make the MOSI and SCK outputs
	SET_BIT(DDRB, SPI_MOSI);
	SET_BIT(DDRB, SPI_SCK);
		
	// make sure the MISO pin is input
	CLEAR_BIT(DDRB, SPI_MISO);
	
	// set all CS lines high (unselected)
	SET_BIT(PORTB, PS_CS);
	SET_BIT(PORTB, DDS_CS);
	SET_BIT(PORTB, LCD_CS);
		
	// make Chip select lines outputs
	SET_BIT(DDRB, PS_CS);				// Is SPI_SS line and must be output for master mode
	SET_BIT(DDRB, DDS_CS);
	SET_BIT(DDRB, DDS_LDAC);
	SET_BIT(DDRB, LCD_CS);
	SET_BIT(DDRB, LCD_A0);
	
	// make Key Rows inputs
	CLEAR_BIT(DDRC, KEY_R4);
	CLEAR_BIT(DDRC, KEY_R3);
	CLEAR_BIT(DDRC, KEY_R2);
	CLEAR_BIT(DDRC, KEY_R1);
	
	// make Key Columns outputs
	SET_BIT(DDRC, KEY_C4);
	SET_BIT(DDRC, KEY_C3);
	SET_BIT(DDRC, KEY_C2);
	SET_BIT(DDRC, KEY_C1);
	
	// make ATX_PWR_ON output
	SET_BIT(DDRD, ATX_PWR_ON);
	
	//make encoder pins inputs
	CLEAR_BIT(DDRD, ENC_PB);
	CLEAR_BIT(DDRD, ENC_B);
	CLEAR_BIT(DDRD, ENC_A);
}