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
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);		// Set ADC clock to 1/128 of XTAL frequency
	ADMUX &= ~(1<<REFS0 | 1<<REFS1);					// Set reference to AREF, Internal Vref turned off
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
	uint16_t adcResult = ADCW;				// ADC conversion result with 10-bit resolution
	uint16_t adcVoltage_mV = (uint16_t)(adcResult * (AVR_VCC_MV / 1024.0f));		// Vin = ADC * Vref / 1024 ; Vref=5.27V
	char adcChannel = (ADMUX & 0x07);		// Lower 3 bits represent the current ADC channel

	switch (adcChannel)
	{
		case 0:
			DevStatus.PS_CURR_mV = adcVoltage_mV / 2.4;		// Ucurr = R24 * (R22 / R23) * IL	=> IL = Ucurr / (R24 * (R22 / R23))
			break;
		case 1:
			DevStatus.PS_VOLT_mV = adcVoltage_mV * 2;
			break;
		case 2:
			DevStatus.ATX_12V_NEG_mV = adcVoltage_mV * 2.4;
			break;
		case 3:
			DevStatus.ATX_12V_mV = adcVoltage_mV * 3; // 2.5;
			break;
		case 4:
			DevStatus.ATX_5V_mV = adcVoltage_mV;
			break;
		case 5:
			DevStatus.ATX_3V3_mV = adcVoltage_mV;
			break;
		case 6:
			DevStatus.DMM1_mV = adcVoltage_mV * 5.17;
			break;
		case 7:
			DevStatus.DMM2_mV = adcVoltage_mV * 5.17;
			break;
		default: break;
	}

	adcChannel++;
	if(adcChannel > 7) { adcChannel = 0; }
	ADMUX = (ADMUX & 0xF8) + adcChannel;	// Set lower bits of ADMUX to select ADC channel
	ADCSRA |= 1<<ADSC;						// Start new ADC conversion if not all channels are converted
}