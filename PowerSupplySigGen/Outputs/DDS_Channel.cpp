/*
 * DDS_Channel.cpp
 *
 * Created: 09.01.2021 16:58:31
 *  Author: V17
 */ 

#include "DDS_Channel.h"
#include <stddef.h>

const char* SignalFormsNames[] = { "SINE", "RECT", "TRIANGLE", "SAWTOOTH" };

DDS_Channel DDS_Channel1;

void DDS_Channel::SetEnabled(bool enabled)
{
	Enabled = enabled;
	DDSUpdateEnabledCallbackFunction();
}

bool DDS_Channel::GetEnabled()
{
	return Enabled;
}

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

void DDS_Channel::SetAmplitude(float amplitude)
{
	Amplitude = amplitude;
	UpdateWaveTable();
}

float DDS_Channel::GetAmplitude()
{
	return Amplitude;
}
		
void DDS_Channel::SetOffset(float offset)
{
	Offset = offset;
	UpdateWaveTable();
}

float DDS_Channel::GetOffset()
{
	return Offset;
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
	UpdateWaveTable();
}

SignalForms_t DDS_Channel::GetSignalForm()
{
	return SignalForm;
}

void DDS_Channel::UpdateWaveTable()
{	
	if(OriginalWaveTable == NULL) { return; }

	int16_t offset_value = (int16_t)((Offset / (float)DDS_AMPLITUDE_MAX) * DDS_SAMPLE_MAX);
	int16_t sample_max_half = DDS_SAMPLE_MAX / 2;

	for(int i = 0; i < (1 << DDS_QUANTIZER_BITS); i++)			// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
	{
		int16_t originalSample = pgm_read_word(&OriginalWaveTable[i]);
		
		// ((Vpp/Vpp_max) * (sample - (sample_max / 2))) + (sample_max / 2) + (Offset/Vpp_max) * sample_max
		int16_t waveTableValue = (int16_t)((Amplitude / (float)DDS_AMPLITUDE_MAX) * (originalSample - sample_max_half)) + sample_max_half + offset_value;

		if(waveTableValue > 4095) { waveTableValue = 4095; }	// Clipping
		else if(waveTableValue < 0) { waveTableValue = 0; }		// Clipping	

		WaveTable[i] = waveTableValue;
	}
}



void DDSUpdateIncrementsCallbackFunction()
{
	DDS_Channel1.SetFrequency(DDS_Channel1.Frequency);		// Use the SetFrequency function to recalculate the new Increment value
}

void DDSUpdateSignalFormsCallbackFunction()
{
	DDS_Channel1.SetSignalForm(DDS_Channel1.SignalForm);	// Use the SetSignalForm function to update the OriginalWaveTable pointer
}

void DDSUpdateWaveTableCallbackFunction()
{
	DDS_Channel1.UpdateWaveTable();
}

void DDSUpdateEnabledCallbackFunction()
{
	if(!DDS_Channel1.GetEnabled())
	{
		DisableDDSTimer();
	}
	else
	{
		InitDDSTimer();
	}
}