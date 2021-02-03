/*
 * PS_Channel.cpp
 *
 * Created: 23.09.2020 19:47:47
 *  Author: V17
 */ 

#include "PS_Channel.h"
#include "../Device.h"

PS_Channel::PS_Channel(float minAmpl, float maxAmpl, float minLoad, float maxLoad) : Channel(POWER_SUPPLY_CHANNEL_TYPE)
{
	Enabled = Parameter<bool>(false, false, true, false, true, true);
	OnEnabledChanged = &PSEnabledChanged;

	Amplitude = Parameter<float>(0, minAmpl, maxAmpl, 0, 1, true);
	OnAmplitudeChanged = &PSAmplitudeChanged;
	
	LoadImpedance = Parameter<float>(0, minLoad, maxLoad, 0, 1, true);
	OnLoadImpedanceChanged = &PSLoadImpedanceChanged;
}

void PS_Channel::UpdateOutput()
{
	if(GetEnabled())
	{
		if(GetLoadImpedance() == 0) { LoadImpedance.Val = LoadImpedance.Min; }
		
		float voltage = ((GetLoadImpedance() + PS_INTERNAL_IMPEDANCE) / GetLoadImpedance()) * GetAmplitude();				//Vset = ((Rload + Rinternal) / Rload) * Vout
		MCP4921_Voltage_Set(voltage / 2);		// divided by two because of OpAmp in circuit that has an amplification of 2
	}
	else
	{
		MCP4921_Voltage_Set(0);
	}
}


void PS_Channel::PSEnabledChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	PS_Channel* psChannel = (PS_Channel*)channel;
	psChannel->UpdateOutput();
	Device.DevSettingsNeedSaving = true;
}

void PS_Channel::PSAmplitudeChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	PS_Channel* psChannel = (PS_Channel*)channel;
	psChannel->UpdateOutput();
	Device.DevSettingsNeedSaving = true;
}

void PS_Channel::PSLoadImpedanceChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	PS_Channel* psChannel = (PS_Channel*)channel;
	psChannel->UpdateOutput();
	Device.DevSettingsNeedSaving = true;
}