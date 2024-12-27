/*
 * Pins.cpp
 * Created: 23.08.2020 21:40:04
 * Author: Markus Scheich
 */ 

#include "Pins.h"

// Initialize the Pins
void Pins_Init()
{
	// The direction of the SPI pins is set by the SPI.begin();
	
	// The key pins are not initialized here. In the KeyPad.h file the complete port C is used directly.

	// ADC pins
	pinMode(PIN_NUMBER_ADC_DVM1, INPUT);
	pinMode(PIN_NUMBER_ADC_DVM2, INPUT);
	pinMode(PIN_NUMBER_ADC_5V, INPUT);
	pinMode(PIN_NUMBER_ADC_3V3, INPUT);
	pinMode(PIN_NUMBER_ADC_PS_VOLT, INPUT);
	pinMode(PIN_NUMBER_ADC_PS_CUR, INPUT);
	
	// Chip selects
	pinMode(PIN_NUMBER_TOUCH_CS, OUTPUT);
	pinMode(PIN_NUMBER_PS_CS, OUTPUT);			// Is SPI_SS line and must be output for master mode
	pinMode(PIN_NUMBER_DDS_CS, OUTPUT);
	pinMode(PIN_NUMBER_IO_EXP_CS, OUTPUT);
	pinMode(PIN_NUMBER_LCD_CS, OUTPUT);

	// Other display and touch pins
	pinMode(PIN_NUMBER_LCD_A0, OUTPUT);
	pinMode(PIN_NUMBER_TOUCH_IRQ, INPUT);

	// Encoder pins	
	pinMode(PIN_NUMBER_ENC_A, INPUT_PULLUP);
	pinMode(PIN_NUMBER_ENC_B, INPUT_PULLUP);
	pinMode(PIN_NUMBER_ENC_PB, INPUT_PULLUP);

	// DMM pins
	pinMode(PIN_NUMBER_DMM1_NEG, INPUT);
	pinMode(PIN_NUMBER_DMM2_NEG, INPUT);

	// IO expander pins
	pinMode(PIN_NUMBER_IO_EXP_INT, INPUT_PULLUP);
}