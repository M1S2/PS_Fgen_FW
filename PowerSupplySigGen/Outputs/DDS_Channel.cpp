/*
 * DDS_Channel.cpp
 *
 * Created: 09.01.2021 16:58:31
 *  Author: V17
 */ 

#include "DDS_Channel.h"
#include "../Device.h"
#include <stddef.h>

#include "../USART/USART.h"

DDS_Channel::DDS_Channel(float minFreq, float maxFreq, float minAmpl, float maxAmpl, float minOffset, float maxOffset) : Channel(DDS_CHANNEL_TYPE), WaveTable{}
{
	Enabled = Parameter<bool>(false, false, true, false, true, true);
	OnEnabledChanged = &DDSEnabledChanged;

	SignalForm = Parameter<SignalForms_t>(SINE, SINE, SAWTOOTH, SINE, SINE, true);
	OnSignalFormChanged = &DDSSignalFormChanged;
	
	Frequency = Parameter<float>(0, minFreq, maxFreq, 0, 1, true);
	OnFrequencyChanged = &DDSFrequencyChanged;
	
	Amplitude = Parameter<float>(0, minAmpl, maxAmpl, 0, 1, true);
	OnAmplitudeChanged = &DDSAmplitudeChanged;
	
	Offset = Parameter<float>(0, minOffset, maxOffset, 0, 1, true);
	OnOffsetChanged = &DDSOffsetChanged;

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

	for(int i = 0; i < (1 << DDS_QUANTIZER_BITS); i++)			// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
	{
		int16_t originalSample = pgm_read_word(&OriginalWaveTable[i]);
		
		// ((Vpp/Vpp_max) * (sample - (sample_max / 2))) + (sample_max / 2) + (Offset/Vpp_max) * sample_max
		int16_t waveTableValue = (int16_t)((GetAmplitude() / (float)DDS_AMPLITUDE_MAX) * (originalSample - sample_max_half)) + sample_max_half + offset_value;

		if(waveTableValue > 4095) { waveTableValue = 4095; }	// Clipping
		else if(waveTableValue < 0) { waveTableValue = 0; }		// Clipping

		WaveTable[i] = waveTableValue;
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
	for (int i = 0; i < NUM_OUTPUT_CHANNELS; i++)
	{
		if (Device.Channels[i].Enabled.Val && Device.Channels[i].GetChannelType() == DDS_CHANNEL_TYPE)
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