/*
 * DDS_Channel.h
 *
 * Created: 09.01.2021 16:58:41
 *  Author: V17
 */ 


#ifndef DDS_CHANNEL_H_
#define DDS_CHANNEL_H_

#include "DDS.h"
#include "Channel.h"
#include "../Configuration.h"

typedef enum SignalForms
{
	SINE,
	RECTANGLE,
	TRIANGLE,
	SAWTOOTH,
	DC,
	#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
		USER_SIGNAL,
	#endif
	NUM_SIGNALFORM_ELEMENTS		// The last element is used to determine the number of elements in the enumeration
} SignalForms_t;
extern const char* SignalFormsNames[NUM_SIGNALFORM_ELEMENTS];

class DDS_Channel : public Channel
{
	public:
		uint8_t DdsChannelNumber;				// 1-based channel number (DDS1 => 1, DDS2 => 2)
	
		Parameter<bool> Enabled;
		Parameter<float> Frequency;
		Parameter<float> Amplitude;
		Parameter<float> Offset;
		Parameter<SignalForms_t> SignalForm;
	
		volatile const uint16_t* OriginalWaveTable;
		volatile uint16_t* p_WaveTable;
		volatile uint32_t* p_Increment;
		
		#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
			uint16_t UserWaveTable[(1 << DDS_QUANTIZER_BITS)];				// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
		#endif

		DDS_Channel(uint8_t ddsChannelNumber, float minFreq, float maxFreq, float minAmpl, float maxAmpl, float minOffset, float maxOffset);
		void UpdateIncrement();
		void UpdateWaveTable();
		void UpdateOriginalWaveTable();

		bool SetEnabled(bool enabled);
		bool GetEnabled();

		bool SetFrequency(float frequency);
		float GetFrequency();

		bool SetAmplitude(float amplitude);
		float GetAmplitude();

		bool SetOffset(float offset);
		float GetOffset();

		bool SetSignalForm(SignalForms_t signalForm);
		SignalForms_t GetSignalForm();

		static void DDSFrequencyChanged(void* channel);
		static void DDSAmplitudeChanged(void* channel);
		static void DDSOffsetChanged(void* channel);
		static void DDSSignalFormChanged(void* channel);
		static void DDSEnabledChanged(void* channel);
};

#endif /* DDS_CHANNEL_H_ */