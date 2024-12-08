/*
 * ADC.cpp
 * Created: 15.09.2020 19:30:49
 * Author: Markus Scheich
 */ 

#include "ADC.h"
#include <avr/interrupt.h>
#include "../Device.h"
#include "../Configuration.h"

void ADC_init()
{
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);		// Set ADC clock to 1/128 of XTAL frequency ("the successive approximation circuitry requires an input clock frequency between 50 kHz and 200 kHz to get maximum resolution")
	ADMUX &= ~(1<<REFS0 | 1<<REFS1);					// Set reference to AREF, Internal Vref turned off
	ADCSRA |= 1<<ADIE;			// Enable ADC Interrupt
	ADCSRA |= 1<<ADEN;			// Enable ADC
}

void ADC_startConversion()
{
	ADMUX = (ADMUX & 0xF8) + 0;	// Set lower 3 bits of ADMUX to 0 to select ADC channel 0	
	ADCSRA |= 1<<ADSC;			// Start ADC conversion
}

ISR(ADC_vect)
{
	cli();
	uint16_t adcResult = ADCW;				// ADC conversion result with 10-bit resolution
	float adcVoltage = (float)(Device.CalibrationFactors.Cal_RefVoltage * (adcResult / 1024.0f));		// Vin = ADC * Vref / 1024 ; Vref=5.27V
	char adcChannel = (ADMUX & 0x07);		// Lower 3 bits represent the current ADC channel

	switch (adcChannel)
	{
	#ifdef PS_SUBSYSTEM_ENABLED
		case 0:
			// Ucurr = R24 * (R22 / R23) * IL	=> IL = Ucurr / (R24 * (R22 / R23))
			Device.PsChannel.MeasuredCurrent = ((adcVoltage / 2.4f) * Device.CalibrationFactors.Cal_PS_CURR) - Device.CalibrationFactors.Cal_PS_CURR_OFFSET;
			if(Device.PsChannel.MeasuredCurrent < 0) { Device.PsChannel.MeasuredCurrent = 0; }
			adcChannel = 1;
			break;
		case 1:
			Device.PsChannel.MeasuredVoltage = adcVoltage * 2 * Device.CalibrationFactors.Cal_PS_VOLT;
			adcChannel = 4;
			break;
	#endif
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		//case 2:
		//	Device.DeviceVoltages.ATX_12V_NEG = -adcVoltage * 2.4f * Device.CalibrationFactors.Cal_ATX_12V_NEG;
		//	break;
		//case 3:
		//	Device.DeviceVoltages.ATX_12V = adcVoltage * 2.5f * Device.CalibrationFactors.Cal_ATX_12V;
		//	break;
		case 4:
			Device.DeviceVoltages.ATX_5V = adcVoltage * Device.CalibrationFactors.Cal_ATX_5V;
			adcChannel = 5;
			break;
		case 5:
			Device.DeviceVoltages.ATX_3V3 = adcVoltage * Device.CalibrationFactors.Cal_ATX_3V3;
			adcChannel = 6;
			break;
		case 6:
			Device.DmmChannel1.MeasuredVoltage = adcVoltage * 5.17f * Device.CalibrationFactors.Cal_DMM1;
			Device.DmmChannel1.IsNegative = BIT_IS_SET(PIND, DMM1_NEG);
			adcChannel = 7;
			break;
		case 7:
			Device.DmmChannel2.MeasuredVoltage = adcVoltage * 5.17f * Device.CalibrationFactors.Cal_DMM2;
			Device.DmmChannel2.IsNegative = BIT_IS_SET(PIND, DMM2_NEG);
			adcChannel = 0;
			break;
	#endif
		default: break;
	}

	ADMUX = (ADMUX & 0xF8) + adcChannel;	// Set lower 3 bits of ADMUX to select ADC channel
	ADCSRA |= 1<<ADSC;						// Start new ADC conversion
	sei();
}