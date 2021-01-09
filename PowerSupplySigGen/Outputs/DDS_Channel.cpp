/*
 * DDS_Channel.cpp
 *
 * Created: 09.01.2021 16:58:31
 *  Author: V17
 */ 

#include "DDS_Channel.h"

const char* SignalFormsNames[] = { "SINE", "RECT", "TRIANGLE", "SAWTOOTH" };

DDS_Channel DDS_Channel1;

void DDS_Channel::SetFrequency(float frequency)
{
	Frequency = frequency;
	
	// increment = 16800	-> 2 kHz
	// increment = 8400		-> 1 kHz
	// increment = 4200		-> 501 Hz
	Increment = (uint16_t)((pow(2, DDS_PHASE_ACCU_BITS) / (float)DDS_TICK_FREQ) * frequency * 2);
}

float DDS_Channel::GetFrequency()
{
	return Frequency;
}

void DDS_Channel::SetSignalForm(SignalForms_t signalForm)
{
	SignalForm = signalForm;
	
	switch(signalForm)
	{
		case SINE: OriginalWaveTable = SINE_WAVE_TABLE_12BIT; break;
		case RECTANGLE: OriginalWaveTable = RECT_WAVE_TABLE_12BIT; break;
		case TRIANGLE: OriginalWaveTable = TRIANGLE_WAVE_TABLE_12BIT; break;
		case SAWTOOTH: OriginalWaveTable = SAWTOOTH_WAVE_TABLE_12BIT; break;
		default: OriginalWaveTable = SINE_WAVE_TABLE_12BIT; break;
	}
}

SignalForms_t DDS_Channel::GetSignalForm()
{
	return SignalForm;
}



void DDSUpdateIncrementsCallbackFunction()
{
	DDS_Channel1.SetFrequency(DDS_Channel1.Frequency);		// Use the SetFrequency function to recalculate the new Increment value
}

void DDSUpdateSignalFormsCallbackFunction()
{
	DDS_Channel1.SetSignalForm(DDS_Channel1.SignalForm);	// Use the SetSignalForm function to update the OriginalWaveTable pointer
}