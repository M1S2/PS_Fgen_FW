/*
 * DDS_Channel.h
 *
 * Created: 09.01.2021 16:58:41
 *  Author: V17
 */ 


#ifndef DDS_CHANNEL_H_
#define DDS_CHANNEL_H_

#include "DDS.h"

typedef enum SignalForms
{
	SINE,
	RECTANGLE,
	TRIANGLE,
	SAWTOOTH
} SignalForms_t;
extern const char* SignalFormsNames[];


class DDS_Channel
{		
	public:
		bool Enabled;
		float Frequency;
		float Amplitude;
		float Offset;
		SignalForms_t SignalForm;
		
		const uint16_t* OriginalWaveTable;
		uint16_t WaveTable[(1 << DDS_QUANTIZER_BITS)];	// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
		uint16_t Accumulator;
		uint16_t Increment = 1024;
		
		DDS_Channel() : WaveTable{}
		{
			Accumulator = 0;
			Increment = 1024;
		}
	
		void SetEnabled(bool enabled);
		bool GetEnabled();
		
		void SetFrequency(float frequency);
		float GetFrequency();
		
		void SetAmplitude(float amplitude);
		float GetAmplitude();
		
		void SetOffset(float offset);
		float GetOffset();
		
		void SetSignalForm(SignalForms_t signalForm);
		SignalForms_t GetSignalForm();
		
		void UpdateWaveTable();
};

extern DDS_Channel DDS_Channel1;

void DDSUpdateIncrementsCallbackFunction();
void DDSUpdateSignalFormsCallbackFunction();
void DDSUpdateWaveTableCallbackFunction();
void DDSUpdateEnabledCallbackFunction();

#endif /* DDS_CHANNEL_H_ */