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
		float Frequency;
		SignalForms_t SignalForm;
		
		const uint16_t* OriginalWaveTable;
		uint16_t Accumulator;
		uint16_t Increment = 1024;
		
		DDS_Channel()
		{
			Accumulator = 0;
			Increment = 1024;
		}
	
		void SetFrequency(float frequency);
		float GetFrequency();
		
		void SetSignalForm(SignalForms_t signalForm);
		SignalForms_t GetSignalForm();
};

extern DDS_Channel DDS_Channel1;

void DDSUpdateIncrementsCallbackFunction();
void DDSUpdateSignalFormsCallbackFunction();

#endif /* DDS_CHANNEL_H_ */