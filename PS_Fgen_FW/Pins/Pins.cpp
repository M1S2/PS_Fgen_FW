/*
 * Pins.cpp
 * Created: 23.08.2020 21:40:04
 * Author: Markus Scheich
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
	
	CLEAR_BIT(PORTB, DDS_LDAC);
		
	// make Chip select lines outputs
	SET_BIT(DDRB, PS_CS);				// Is SPI_SS line and must be output for master mode
	SET_BIT(DDRB, DDS_CS);
	SET_BIT(DDRB, DDS_LDAC);
	SET_BIT(DDRB, LCD_CS);
	SET_BIT(DDRB, LCD_A0);
	
	// make Key Rows outputs
	SET_BIT(DDRC, KEY_R4);
	SET_BIT(DDRC, KEY_R3);
	SET_BIT(DDRC, KEY_R2);
	SET_BIT(DDRC, KEY_R1);
	
	// power the row pins
	SET_BIT(PORTC, KEY_R4);
	SET_BIT(PORTC, KEY_R3);
	SET_BIT(PORTC, KEY_R2);
	SET_BIT(PORTC, KEY_R1);
	
	// make Key Columns inputs
	CLEAR_BIT(DDRC, KEY_C4);
	CLEAR_BIT(DDRC, KEY_C3);
	CLEAR_BIT(DDRC, KEY_C2);
	CLEAR_BIT(DDRC, KEY_C1);
	
	// make encoder pins inputs
	CLEAR_BIT(DDRD, ENC_PB);
	CLEAR_BIT(DDRD, ENC_B);
	CLEAR_BIT(DDRD, ENC_A);
	
	// enable pull-ups for encoder input pins
	SET_BIT(PORTD, ENC_A);
	SET_BIT(PORTD, ENC_B);
	SET_BIT(PORTD, ENC_PB);
}