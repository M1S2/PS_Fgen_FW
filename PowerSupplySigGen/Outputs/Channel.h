/*
 * Channel.h
 *
 * Created: 03.02.2021 20:39:32
 *  Author: V17
 */ 


#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "Parameter.h"

typedef enum SignalForms
{
	SINE,
	RECTANGLE,
	TRIANGLE,
	SAWTOOTH
} SignalForms_t;
extern const char* SignalFormsNames[];

typedef enum ChannelTypes
{
	POWER_SUPPLY_CHANNEL_TYPE,
	DDS_CHANNEL_TYPE
} ChannelTypes_t;


class Channel
{
	protected:
		void(*OnEnabledChanged)(void* channel);
		void(*OnFrequencyChanged)(void* channel);
		void(*OnAmplitudeChanged)(void* channel);
		void(*OnOffsetChanged)(void* channel);
		void(*OnSignalFormChanged)(void* channel);
		void(*OnLoadImpedanceChanged)(void* channel);

		ChannelTypes_t _channelType;

	public:
		Channel(ChannelTypes_t channelType)
		{
			_channelType = channelType;
			OnEnabledChanged = nullptr;
			OnFrequencyChanged = nullptr;
			OnAmplitudeChanged = nullptr;
			OnOffsetChanged = nullptr;
			OnSignalFormChanged = nullptr;
			OnLoadImpedanceChanged = nullptr;
		}

		Parameter<bool> Enabled;
		Parameter<float> Frequency;
		Parameter<float> Amplitude;
		Parameter<float> Offset;
		Parameter<SignalForms_t> SignalForm;
		Parameter<float> LoadImpedance;

		ChannelTypes_t GetChannelType();

		bool SetEnabled(bool enabled);
		bool GetEnabled();
		bool IsEnabledSupported();

		bool SetFrequency(float frequency);
		float GetFrequency();
		bool IsFrequencySupported();

		bool SetAmplitude(float amplitude);
		float GetAmplitude();
		bool IsAmplitudeSupported();

		bool SetOffset(float offset);
		float GetOffset();
		bool IsOffsetSupported();

		bool SetSignalForm(SignalForms_t signalForm);
		SignalForms_t GetSignalForm();
		bool IsSignalFormSupported();

		bool SetLoadImpedance(float loadImpedance);
		float GetLoadImpedance();
		bool IsLoadImpedanceSupported();
};

#endif /* CHANNEL_H_ */