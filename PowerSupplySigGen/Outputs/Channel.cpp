/*
 * Channel.cpp
 *
 * Created: 03.02.2021 20:40:24
 *  Author: V17
 */ 

#include "Channel.h"

const char* SignalFormsNames[] = { "SINE", "RECT", "TRIANGLE", "SAWTOOTH" };

ChannelTypes_t Channel::GetChannelType()
{
	return _channelType;
}

//----------------------------------------------------------------------------------------------------------

bool Channel::SetEnabled(bool enabled)
{
	if (!IsEnabledSupported()) { return false; }

	if (Enabled.Val != enabled)
	{
		Enabled.Val = enabled;
		if (OnEnabledChanged != nullptr) { OnEnabledChanged(this); }
	}
	return true;
}

bool Channel::GetEnabled()
{
	return Enabled.Val;
}

bool Channel::IsEnabledSupported()
{
	return Enabled.Supported;
}

//----------------------------------------------------------------------------------------------------------

bool Channel::SetFrequency(float frequency)
{
	if (!IsFrequencySupported() || frequency > Frequency.Max || frequency < Frequency.Min) { return false; }

	if (Frequency.Val != frequency)
	{
		Frequency.Val = frequency;
		if (OnFrequencyChanged != nullptr) { OnFrequencyChanged(this); }
	}
	return true;
}

float Channel::GetFrequency()
{
	return Frequency.Val;
}

bool Channel::IsFrequencySupported()
{
	return Frequency.Supported;
}

//----------------------------------------------------------------------------------------------------------

bool Channel::SetAmplitude(float amplitude)
{
	if (!IsAmplitudeSupported() || amplitude > Amplitude.Max || amplitude < Amplitude.Min) { return false; }

	if (Amplitude.Val != amplitude)
	{
		Amplitude.Val = amplitude;
		if (OnAmplitudeChanged != nullptr) { OnAmplitudeChanged(this); }
	}
	return true;
}

float Channel::GetAmplitude()
{
	return Amplitude.Val;
}

bool Channel::IsAmplitudeSupported()
{
	return Amplitude.Supported;
}

//----------------------------------------------------------------------------------------------------------

bool Channel::SetOffset(float offset)
{
	if (!IsOffsetSupported() || offset > Offset.Max || offset < Offset.Min) { return false; }

	if (Offset.Val != offset)
	{
		Offset.Val = offset;
		if (OnOffsetChanged != nullptr) { OnOffsetChanged(this); }
	}
	return true;
}

float Channel::GetOffset()
{
	return Offset.Val;
}

bool Channel::IsOffsetSupported()
{
	return Offset.Supported;
}

//----------------------------------------------------------------------------------------------------------

bool Channel::SetSignalForm(SignalForms_t signalForm)
{
	if (!IsSignalFormSupported() || signalForm > SignalForm.Max || signalForm < SignalForm.Min) { return false; }

	if (SignalForm.Val != signalForm)
	{
		SignalForm.Val = signalForm;
		if (OnSignalFormChanged != nullptr) { OnSignalFormChanged(this); }
	}
	return true;
}

SignalForms_t Channel::GetSignalForm()
{
	return SignalForm.Val;
}

bool Channel::IsSignalFormSupported()
{
	return SignalForm.Supported;
}

//----------------------------------------------------------------------------------------------------------

bool Channel::SetLoadImpedance(float loadImpedance)
{
	if (!IsLoadImpedanceSupported() || loadImpedance > LoadImpedance.Max || loadImpedance < LoadImpedance.Min) { return false; }

	if (LoadImpedance.Val != loadImpedance)
	{
		LoadImpedance.Val = loadImpedance;
		if (OnLoadImpedanceChanged != nullptr) { OnLoadImpedanceChanged(this); }
	}
	return true;
}

float Channel::GetLoadImpedance()
{
	return LoadImpedance.Val;
}

bool Channel::IsLoadImpedanceSupported()
{
	return LoadImpedance.Supported;
}