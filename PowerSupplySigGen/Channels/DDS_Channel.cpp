/*
 * DDS_Channel.cpp
 *
 * Created: 09.01.2021 16:58:31
 *  Author: V17
 */ 

#include "DDS_Channel.h"
#include "../Device.h"
#include <stddef.h>

DDS_Channel::DDS_Channel(float minFreq, float maxFreq, float minAmpl, float maxAmpl, float minOffset, float maxOffset) : Channel(DDS_CHANNEL_TYPE), WaveTable{0}
{
	Enabled = Parameter<bool>(false, false, true, false, true);
	SignalForm = Parameter<SignalForms_t>(SINE, SINE, SAWTOOTH, SINE, SINE);
	Frequency = Parameter<float>(0, minFreq, maxFreq, 0, 1);
	Amplitude = Parameter<float>(0, minAmpl, maxAmpl, 0, 1);
	Offset = Parameter<float>(0, minOffset, maxOffset, 0, 1);

	Accumulator = 0;
	Increment = 1024;
}

void DDS_Channel::UpdateIncrement()
{
	// increment = 16800	-> 2 kHz
	// increment = 8400		-> 1 kHz
	// increment = 4200		-> 501 Hz
	Increment = (uint16_t)((pow(2, DDS_PHASE_ACCU_BITS) / (float)DDS_TICK_FREQ) * GetFrequency() * 2);
}

void DDS_Channel::UpdateWaveTable()
{		
	if(OriginalWaveTable == NULL) { return; }

	int16_t offset_value = (int16_t)((GetOffset() / (float)DDS_AMPLITUDE_MAX) * DDS_SAMPLE_MAX);
	int16_t sample_max_half = DDS_SAMPLE_MAX / 2;
	
	for(uint16_t i = 0; i < (1 << DDS_QUANTIZER_BITS); i++)			// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
	{
		int16_t originalSample = pgm_read_word(&OriginalWaveTable[i]);
		
		// ((Vpp/Vpp_max) * (sample - (sample_max / 2))) + (sample_max / 2) + (Offset/Vpp_max) * sample_max
		int16_t waveTableValue = (int16_t)((GetAmplitude() / (float)DDS_AMPLITUDE_MAX) * (originalSample - sample_max_half)) + sample_max_half + offset_value;

		if(waveTableValue > 4095) { waveTableValue = 4095; }	// Clipping
		else if(waveTableValue < 0) { waveTableValue = 0; }		// Clipping

		WaveTable[i] = (uint16_t)waveTableValue;
	}
}

void DDS_Channel::UpdateOriginalWaveTable()
{	
	switch (SignalForm.Val)
	{
		case SINE: OriginalWaveTable = SINE_WAVE_TABLE_12BIT; break;
		case RECTANGLE: OriginalWaveTable = RECT_WAVE_TABLE_12BIT; break;
		case TRIANGLE: OriginalWaveTable = TRIANGLE_WAVE_TABLE_12BIT; break;
		case SAWTOOTH: OriginalWaveTable = SAWTOOTH_WAVE_TABLE_12BIT; break;
		default: OriginalWaveTable = SINE_WAVE_TABLE_12BIT; break;
	}
}

//----------------------------------------------------------------------------------------------------------

bool DDS_Channel::SetEnabled(bool enabled)
{
	if (Enabled.Val != enabled)
	{
		Enabled.Val = enabled;
		DDSEnabledChanged(this);
	}
	return true;
}

bool DDS_Channel::GetEnabled()
{
	return Enabled.Val;
}

//----------------------------------------------------------------------------------------------------------

bool DDS_Channel::SetFrequency(float frequency)
{
	if (frequency > Frequency.Max || frequency < Frequency.Min) { return false; }

	if (Frequency.Val != frequency)
	{
		Frequency.Val = frequency;
		DDSFrequencyChanged(this);
	}
	return true;
}

float DDS_Channel::GetFrequency()
{
	return Frequency.Val;
}

//----------------------------------------------------------------------------------------------------------

bool DDS_Channel::SetAmplitude(float amplitude)
{
	if (amplitude > Amplitude.Max || amplitude < Amplitude.Min) { return false; }

	if (Amplitude.Val != amplitude)
	{
		Amplitude.Val = amplitude;
		DDSAmplitudeChanged(this);
	}
	return true;
}

float DDS_Channel::GetAmplitude()
{
	return Amplitude.Val;
}

//----------------------------------------------------------------------------------------------------------

bool DDS_Channel::SetOffset(float offset)
{
	if (offset > Offset.Max || offset < Offset.Min) { return false; }

	if (Offset.Val != offset)
	{
		Offset.Val = offset;
		DDSOffsetChanged(this);
	}
	return true;
}

float DDS_Channel::GetOffset()
{
	return Offset.Val;
}

//----------------------------------------------------------------------------------------------------------

bool DDS_Channel::SetSignalForm(SignalForms_t signalForm)
{
	if (signalForm > SignalForm.Max || signalForm < SignalForm.Min) { return false; }

	if (SignalForm.Val != signalForm)
	{
		SignalForm.Val = signalForm;
		DDSSignalFormChanged(this);
	}
	return true;
}

SignalForms_t DDS_Channel::GetSignalForm()
{
	return SignalForm.Val;
}

//----------------------------------------------------------------------------------------------------------

void DDS_Channel::DDSFrequencyChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != DDS_CHANNEL_TYPE) { return;  }
	DDS_Channel* ddsChannel = (DDS_Channel*)channel;
	ddsChannel->UpdateIncrement();
	Device.DevSettingsNeedSaving = true;
}

void DDS_Channel::DDSAmplitudeChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != DDS_CHANNEL_TYPE) { return; }
	DDS_Channel* ddsChannel = (DDS_Channel*)channel;
	ddsChannel->UpdateWaveTable();
	Device.DevSettingsNeedSaving = true;
}

void DDS_Channel::DDSOffsetChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != DDS_CHANNEL_TYPE) { return; }
	DDS_Channel* ddsChannel = (DDS_Channel*)channel;
	ddsChannel->UpdateWaveTable();
	Device.DevSettingsNeedSaving = true;
}

void DDS_Channel::DDSSignalFormChanged(void* channel)
{	
	if (((Channel*)channel)->GetChannelType() != DDS_CHANNEL_TYPE) { return; }
	DDS_Channel* ddsChannel = (DDS_Channel*)channel;
	ddsChannel->UpdateOriginalWaveTable();
	ddsChannel->UpdateWaveTable();
	Device.DevSettingsNeedSaving = true;
}

void DDS_Channel::DDSEnabledChanged(void* channel)
{
	bool areDDSChannelsEnabled = false;
	for (int i = 0; i < NUM_CHANNELS; i++)
	{
		if (Device.Channels[i]->GetChannelType() == DDS_CHANNEL_TYPE && ((DDS_Channel*)Device.Channels[i])->Enabled.Val)
		{
			areDDSChannelsEnabled = true;
		}
	}

	if (areDDSChannelsEnabled)
	{
		InitDDSTimer();
	}
	else
	{
		DisableDDSTimer();
	}

	Device.DevSettingsNeedSaving = true;
}