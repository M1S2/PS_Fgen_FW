/*
 * DDS_Channel.cpp
 * Created: 09.01.2021 16:58:31
 * Author: Markus Scheich
 */ 

#include "DDS_Channel.h"
#include "../Device.h"
#include <stddef.h>

#ifdef DDS_SUBSYSTEM_ENABLED

const char* SignalFormsNames[] = { "SINE", "RECT", "TRIANGLE", "SAWTOOTH", "DC", "PWM" 
	#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
		, "USER"
	#endif
	};

DDS_Channel::DDS_Channel(uint8_t ddsChannelNumber, float minFreq, float maxFreq, float minAmpl, float maxAmpl, float minOffset, float maxOffset) : Channel(DDS_CHANNEL_TYPE)
{
	DdsChannelNumber = ddsChannelNumber;
	
	Enabled = Parameter<bool>(false, false, true, false, true);
	SignalForm = Parameter<SignalForms_t>(SIGNALFORM_SINE, SIGNALFORM_SINE, (SignalForms_t)(((int)NUM_SIGNALFORM_ELEMENTS) - 1), SIGNALFORM_SINE, SIGNALFORM_SINE);
	Frequency = Parameter<float>(0, minFreq, maxFreq, 1000, 1);
	Amplitude = Parameter<float>(0, minAmpl, maxAmpl, 10, 1);
	Offset = Parameter<float>(0, minOffset, maxOffset, 0, 1);
	PWM_Value = Parameter<float>(50, 0, 100, 50, 1);

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
		if(OriginalWaveTable == NULL && SignalForm.Val != SIGNALFORM_USER_SIGNAL && SignalForm.Val != SIGNALFORM_PWM && SignalForm.Val != SIGNALFORM_RECTANGLE && SignalForm.Val != SIGNALFORM_DC) { return; }
	#else
		if(OriginalWaveTable == NULL && SignalForm.Val != SIGNALFORM_PWM && SignalForm.Val != SIGNALFORM_RECTANGLE && SignalForm.Val != SIGNALFORM_DC) { return; }
	#endif

	int16_t offset_value = (int16_t)((GetOffset() / (float)DDS_AMPLITUDE_MAX) * DDS_SAMPLE_MAX);
	int16_t sample_max_half = DDS_SAMPLE_MAX / 2;
	
	uint16_t pwm_stateChangeIndex = 0;								// Index in the wave table where the PWM signal changes it's state (between 0 and 2^DDS_QUANTIZER_BITS). This is only used for SignalForm == PWM and SignalForm == RECTANGLE (PWM with 50%)
	if(SignalForm.Val == SIGNALFORM_PWM)
	{
		pwm_stateChangeIndex = (PWM_Value.Val * (1 << DDS_QUANTIZER_BITS)) / 100;
	}
	else if(SignalForm.Val == SIGNALFORM_RECTANGLE)
	{
		pwm_stateChangeIndex = (50 * (1 << DDS_QUANTIZER_BITS)) / 100;
	}
	
	for(uint16_t i = 0; i < (1 << DDS_QUANTIZER_BITS); i++)			// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
	{
		int16_t originalSample = 0;
		#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
			if(SignalForm.Val == SIGNALFORM_USER_SIGNAL)
			{
				originalSample = UserWaveTable[i];
			}
			else
			{
		#endif
				if(SignalForm.Val == SIGNALFORM_PWM || SignalForm.Val == SIGNALFORM_RECTANGLE)		// Rectangle is a special case of the PWM signal form (PWM with 50% duty cycle)
				{
					originalSample = (i < pwm_stateChangeIndex) ? 0xFFF : 0x000;
				}
				else if(SignalForm.Val == SIGNALFORM_DC)
				{
					originalSample = 0xFFF;
				}
				else
				{
					originalSample = pgm_read_word(&OriginalWaveTable[i]);
				}
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
		case SIGNALFORM_SINE: OriginalWaveTable = SINE_WAVE_TABLE_12BIT; break;
		case SIGNALFORM_RECTANGLE: OriginalWaveTable = NULL; break;	// The RECTANGLE SignalForm doesn't use an OriginalWaveTable. The waveform is directly calculated in the UpdateWaveTable function
		case SIGNALFORM_TRIANGLE: OriginalWaveTable = TRIANGLE_WAVE_TABLE_12BIT; break;
		case SIGNALFORM_SAWTOOTH: OriginalWaveTable = SAWTOOTH_WAVE_TABLE_12BIT; break;
		case SIGNALFORM_DC: OriginalWaveTable = NULL; break;			// The DC SignalForm doesn't use an OriginalWaveTable. The waveform is directly calculated in the UpdateWaveTable function
		case SIGNALFORM_PWM: OriginalWaveTable = NULL; break;			// The PWM SignalForm doesn't use an OriginalWaveTable. The waveform is directly calculated in the UpdateWaveTable function
		#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
			case SIGNALFORM_USER_SIGNAL: OriginalWaveTable = NULL; break;
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

//----------------------------------------------------------------------------------------------------------

bool DDS_Channel::SetPWMValue(float pwmValue)
{
	if (pwmValue > PWM_Value.Max || pwmValue < PWM_Value.Min) { return false; }

	if (PWM_Value.Val != pwmValue)
	{
		PWM_Value.Val = pwmValue;
		DDSPWMValueChanged(this);
	}
	return true;
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

void DDS_Channel::DDSPWMValueChanged(void* channel)
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
		dds_channel1_enabled = ((DDS_Channel*)channel)->GetEnabled();
		
		OnOffControls_SetLEDState(ONOFFLED_DDS1, dds_channel1_enabled);
	}
	else if(((DDS_Channel*)channel)->DdsChannelNumber == 2) 
	{ 
		DisableDDS2(); 
		dds_channel2_enabled = ((DDS_Channel*)channel)->GetEnabled();
		
		OnOffControls_SetLEDState(ONOFFLED_DDS2, dds_channel2_enabled);
	}
	
	bool areDDSChannelsEnabled = false;
	for (int i = 0; i < NUM_CHANNELS; i++)
	{
		if (Device.Channels[i]->GetChannelType() == DDS_CHANNEL_TYPE && ((DDS_Channel*)Device.Channels[i])->GetEnabled())
		{
			areDDSChannelsEnabled = true;
		}
	}

	if (areDDSChannelsEnabled)
	{
		StartDDSTimer();
	}
	else
	{
		DisableDDSTimer();
		DisableDDS1();
		DisableDDS2();
	}

	Device.SetSettingsChanged(true);
}

#endif /* DDS_SUBSYSTEM_ENABLED */