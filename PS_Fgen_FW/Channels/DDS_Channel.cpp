/*
 * DDS_Channel.cpp
 *
 * Created: 09.01.2021 16:58:31
 *  Author: V17
 */ 

#include "DDS_Channel.h"
#include "../Device.h"
#include <stddef.h>

const char* SignalFormsNames[] = { "SINE", "RECT", "TRIANGLE", "SAWTOOTH", "DC", 
	#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
		"USER"
	#endif
	};

DDS_Channel::DDS_Channel(uint8_t ddsChannelNumber, float minFreq, float maxFreq, float minAmpl, float maxAmpl, float minOffset, float maxOffset) : Channel(DDS_CHANNEL_TYPE)
{
	DdsChannelNumber = ddsChannelNumber;
	
	Enabled = Parameter<bool>(false, false, true, false, true);
	SignalForm = Parameter<SignalForms_t>(SINE, SINE, SAWTOOTH, SINE, SINE);
	Frequency = Parameter<float>(0, minFreq, maxFreq, 1000, 1);
	Amplitude = Parameter<float>(0, minAmpl, maxAmpl, 10, 1);
	Offset = Parameter<float>(0, minOffset, maxOffset, 0, 1);

	if(DdsChannelNumber == 1)
	{
		p_Increment = &dds_channel1_increment;
		p_WaveTable = dds_channel1_waveTable;
	}
	else
	{
		p_Increment = &dds_channel2_increment;
		p_WaveTable = dds_channel2_waveTable;
	}
	*p_Increment = 1024;
}

void DDS_Channel::UpdateIncrement()
{
	*p_Increment = (uint32_t)((pow(2, DDS_PHASE_ACCU_BITS) / (float)DDS_TICK_FREQ) * GetFrequency() * Device.CalibrationFactors.Cal_DDS_FREQ);
}

void DDS_Channel::UpdateWaveTable()
{
	#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
		if(OriginalWaveTable == NULL && SignalForm.Val != USER_SIGNAL) { return; }
	#else
		if(OriginalWaveTable == NULL) { return; }
	#endif

	int16_t offset_value = (int16_t)((GetOffset() / (float)DDS_AMPLITUDE_MAX) * DDS_SAMPLE_MAX);
	int16_t sample_max_half = DDS_SAMPLE_MAX / 2;
	
	for(uint16_t i = 0; i < (1 << DDS_QUANTIZER_BITS); i++)			// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
	{
		int16_t originalSample = 0;
		#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
			if(SignalForm.Val == USER_SIGNAL)
			{
				originalSample = UserWaveTable[i];
			}
			else
			{
		#endif 
				originalSample = pgm_read_word(&OriginalWaveTable[i]);
		#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
			}
		#endif
		
		// ((Vpp/Vpp_max) * (sample - (sample_max / 2))) + (sample_max / 2) + (Offset/Vpp_max) * sample_max
		int16_t waveTableValue = (int16_t)((GetAmplitude() / (float)DDS_AMPLITUDE_MAX) * (originalSample - sample_max_half)) + sample_max_half + offset_value;

		if(waveTableValue > 4095) { waveTableValue = 4095; }	// Clipping
		else if(waveTableValue < 0) { waveTableValue = 0; }		// Clipping

		p_WaveTable[i] = (uint16_t)waveTableValue;
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
		case DC: OriginalWaveTable = DC_WAVE_TABLE_12BIT; break;
		#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
			case USER_SIGNAL: OriginalWaveTable = NULL; break;
		#endif
		default: OriginalWaveTable = SINE_WAVE_TABLE_12BIT; break;
	}
}

//----------------------------------------------------------------------------------------------------------

bool DDS_Channel::SetEnabled(bool enabled)
{
	Enabled.Val = enabled;
	DDSEnabledChanged(this);
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
	Device.SetSettingsChanged(true);
}

void DDS_Channel::DDSAmplitudeChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != DDS_CHANNEL_TYPE) { return; }
	DDS_Channel* ddsChannel = (DDS_Channel*)channel;
	ddsChannel->UpdateWaveTable();
	Device.SetSettingsChanged(true);
}

void DDS_Channel::DDSOffsetChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != DDS_CHANNEL_TYPE) { return; }
	DDS_Channel* ddsChannel = (DDS_Channel*)channel;
	ddsChannel->UpdateWaveTable();
	Device.SetSettingsChanged(true);
}

void DDS_Channel::DDSSignalFormChanged(void* channel)
{	
	if (((Channel*)channel)->GetChannelType() != DDS_CHANNEL_TYPE) { return; }
	DDS_Channel* ddsChannel = (DDS_Channel*)channel;
	ddsChannel->UpdateOriginalWaveTable();
	ddsChannel->UpdateWaveTable();
	Device.SetSettingsChanged(true);
}

void DDS_Channel::DDSEnabledChanged(void* channel)
{
	if(((DDS_Channel*)channel)->DdsChannelNumber == 1) 
	{
		DisableDDS1(); 
		dds_channel1_enabled = ((DDS_Channel*)channel)->Enabled.Val;
	}
	else if(((DDS_Channel*)channel)->DdsChannelNumber == 2) 
	{ 
		DisableDDS2(); 
		dds_channel2_enabled = ((DDS_Channel*)channel)->Enabled.Val;
	}
	
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

	Device.SetSettingsChanged(true);
}