/*
 * ADC.cpp
 *
 * Created: 15.09.2020 19:30:49
 *  Author: V17
 */ 

#include "ADC.h"
#include <avr/interrupt.h>

void ADC_init()
{
	ADCSRA |= 1<<ADPS2;			// Set ADC clock to 1/16 of XTAL frequency
	ADMUX &= ~(1<<REFS0 | 1<<REFS1);	// Set reference to AREF, Internal Vref turned off
	ADCSRA |= 1<<ADIE;			// Enable ADC Interrupt
	ADCSRA |= 1<<ADEN;			// Enable ADC

	//After ADC_init():
	//Enable Interrupts: sei();
	//Then start first conversion: ADC_startFirstConversion();
}

void ADC_startConversion()
{
	ADCSRA |= 1<<ADSC;			// Start ADC conversion
}

ISR(ADC_vect)
{
	uint16_t adcResult = ADCL | (ADCH<<8);	// ADC conversion result with 10-bit resolution
	float adcVoltage = ((float)adcResult / 1024) * 5;	// Vin = ADC * Vref / 1024 ; Vref=5V
	char adcChannel = (ADMUX & 0x07);	// Lower 3 bits represent the current ADC channel

	switch (adcChannel)
	{
		case 0:
			DevStatus.PS_CURR = adcVoltage / 2.4;		// Ucurr = R24 * (R22 / R23) * IL	=> IL = Ucurr / (R24 * (R22 / R23))
			break;
		case 1:
			DevStatus.PS_VOLT = adcVoltage * 2;
			break;
		case 2:
			DevStatus.ATX_12V_NEG = adcVoltage * -2.4;// factor ?
			break;
		case 3:
			DevStatus.ATX_12V = adcVoltage * 2.4;	// factor ?
			break;
		case 4:
			DevStatus.ATX_5V = adcVoltage;
			break;
		case 5:
			DevStatus.ATX_3V3 = adcVoltage;
			break;
		case 6:
			DevStatus.DMM1 = adcVoltage * 2;	// factor ?
			break;
		case 7:
			DevStatus.DMM2 = adcVoltage * 2;	// factor ?
			break;
		default: break;
	}
	
	char currentChannel = adcChannel;
	adcChannel++;
	if(adcChannel > 7) { adcChannel = 0; }
	ADMUX = (ADMUX & 0xF8) + adcChannel;	// Set lower bits of ADMUX to select ADC channel
	if(currentChannel < 7) { ADCSRA |= 1<<ADSC; }			// Start new ADC conversion if not all channels are converted
}