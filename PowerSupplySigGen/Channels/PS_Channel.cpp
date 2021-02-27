/*
 * PS_Channel.cpp
 *
 * Created: 23.09.2020 19:47:47
 *  Author: V17
 */ 

#include "PS_Channel.h"
#include "../Device.h"

PS_Channel::PS_Channel(float minAmpl, float maxAmpl, float minCurrent, float maxCurrent, float minLoad, float maxLoad, uint8_t minOvpLevel, uint8_t maxOvpLevel, float minOvpDelay, float maxOvpDelay, uint8_t minOcpLevel, uint8_t maxOcpLevel, float minOcpDelay, float maxOcpDelay) : Channel(POWER_SUPPLY_CHANNEL_TYPE)
{
	Enabled = Parameter<bool>(false, false, true, false, true);
	Amplitude = Parameter<float>(0, minAmpl, maxAmpl, 5, 1);
	Current = Parameter<float>(0, minCurrent, maxCurrent, 0.1, 0.1);
	LoadImpedance = Parameter<float>(0, minLoad, maxLoad, 1000000, 1);
	
	OvpState = Parameter<bool>(false, false, true, false, true);
	OvpLevel = Parameter<uint8_t>(0, minOvpLevel, maxOvpLevel, 120, 1);
	OvpDelay = Parameter<float>(0.1, minOvpDelay, maxOvpDelay, 0.1, 0.1);
	
	OcpState = Parameter<bool>(false, false, true, false, true);
	OcpLevel = Parameter<uint8_t>(0, minOcpLevel, maxOcpLevel, 120, 1);
	OcpDelay = Parameter<float>(0.1, minOcpDelay, maxOcpDelay, 0.1, 0.1);
	
	TimeCounter_OvpDelay_ms = 0;
	TimeCounter_OcpDelay_ms = 0;
	_psState = PS_STATE_CV;
}

void PS_Channel::SwitchOffOutput()
{
	MCP4921_Voltage_Set(0);
}
		
void PS_Channel::UpdateOutput()
{
	if(GetEnabled() && _psState == PS_STATE_CV)
	{
		if(GetLoadImpedance() == 0) { LoadImpedance.Val = LoadImpedance.Min; }
		
		float voltage = ((GetLoadImpedance() + PS_INTERNAL_IMPEDANCE) / GetLoadImpedance()) * GetAmplitude();				//Vset = ((Rload + Rinternal) / Rload) * Vout
		MCP4921_Voltage_Set(voltage / 2);		// divided by two because of OpAmp in circuit that has an amplification of 2
	}
	else
	{
		SwitchOffOutput();
	}
}

void PS_Channel::DeviceTimerTickISR(uint16_t currentPeriod_ms)
{
	if(GetOvpState() && MeasuredAmplitude > (GetAmplitude() * (GetOvpLevel() / 100.0f)))
	{
		TimeCounter_OvpDelay_ms += currentPeriod_ms;
	}
	else { TimeCounter_OvpDelay_ms = 0; }
	
	if(GetOcpState() && MeasuredCurrent > (GetCurrent() * (GetOcpLevel() / 100.0f)))
	{
		TimeCounter_OcpDelay_ms += currentPeriod_ms;
	}
	else { TimeCounter_OcpDelay_ms = 0; }

	if(TimeCounter_OvpDelay_ms >= (1000 * GetOvpDelay()))
	{
		_psState = PS_STATE_OVP;
		TimeCounter_OvpDelay_ms = 0;
		SwitchOffOutput();
	}
	else if(TimeCounter_OcpDelay_ms >= (1000 * GetOcpDelay()))
	{
		_psState = PS_STATE_OCP;
		TimeCounter_OcpDelay_ms = 0;
		SwitchOffOutput();
	}

	/* ... OPP ...*/
}

void PS_Channel::ClearProtections()
{
	if(_psState == PS_STATE_OVP)
	{
		_psState = PS_STATE_CV;
		TimeCounter_OvpDelay_ms = 0;
		UpdateOutput();
	}
	else if(_psState == PS_STATE_OCP)
	{
		_psState = PS_STATE_CV;
		TimeCounter_OcpDelay_ms = 0;
		UpdateOutput();
	}
	else if(_psState == PS_STATE_OPP)
	{
		/* ... */
	}
}

PsStates_t PS_Channel::GetPsState()
{
	return _psState;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetEnabled(bool enabled)
{
	if (Enabled.Val != enabled)
	{
		Enabled.Val = enabled;
		PSEnabledChanged(this);
	}
	return true;
}

bool PS_Channel::GetEnabled()
{
	return Enabled.Val;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetAmplitude(float amplitude)
{
	if (amplitude > Amplitude.Max || amplitude < Amplitude.Min) { return false; }

	if (Amplitude.Val != amplitude)
	{
		Amplitude.Val = amplitude;
		PSAmplitudeChanged(this);
	}
	return true;
}

float PS_Channel::GetAmplitude()
{
	return Amplitude.Val;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetCurrent(float current)
{
	if (current > Current.Max || current < Current.Min) { return false; }

	if (Current.Val != current)
	{
		Current.Val = current;
		PSCurrentChanged(this);
	}
	return true;
}

float PS_Channel::GetCurrent()
{
	return Current.Val;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetLoadImpedance(float loadImpedance)
{
	if (loadImpedance > LoadImpedance.Max || loadImpedance < LoadImpedance.Min) { return false; }

	if (LoadImpedance.Val != loadImpedance)
	{
		LoadImpedance.Val = loadImpedance;
		PSLoadImpedanceChanged(this);
	}
	return true;
}

float PS_Channel::GetLoadImpedance()
{
	return LoadImpedance.Val;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetOvpLevel(uint8_t ovpLevel)
{
	if (ovpLevel > OvpLevel.Max || ovpLevel < OvpLevel.Min) { return false; }

	if (OvpLevel.Val != ovpLevel)
	{
		OvpLevel.Val = ovpLevel;
		PSOvpLevelChanged(this);
	}
	return true;
}

uint8_t PS_Channel::GetOvpLevel()
{
	return OvpLevel.Val;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetOvpState(bool ovpState)
{
	if (ovpState > OvpState.Max || ovpState < OvpState.Min) { return false; }

	if (OvpState.Val != ovpState)
	{
		OvpState.Val = ovpState;
		PSOvpStateChanged(this);
	}
	return true;
}

bool PS_Channel::GetOvpState()
{
	return OvpState.Val;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetOvpDelay(float ovpDelay)
{
	if (ovpDelay > OvpDelay.Max || ovpDelay < OvpDelay.Min) { return false; }

	if (OvpDelay.Val != ovpDelay)
	{
		OvpDelay.Val = ovpDelay;
		PSOvpDelayChanged(this);
	}
	return true;
}

float PS_Channel::GetOvpDelay()
{
	return OvpDelay.Val;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetOcpLevel(uint8_t ocpLevel)
{
	if (ocpLevel > OcpLevel.Max || ocpLevel < OcpLevel.Min) { return false; }

	if (OcpLevel.Val != ocpLevel)
	{
		OcpLevel.Val = ocpLevel;
		PSOcpLevelChanged(this);
	}
	return true;
}

uint8_t PS_Channel::GetOcpLevel()
{
	return OcpLevel.Val;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetOcpState(bool ocpState)
{
	if (ocpState > OcpState.Max || ocpState < OcpState.Min) { return false; }

	if (OcpState.Val != ocpState)
	{
		OcpState.Val = ocpState;
		PSOcpStateChanged(this);
	}
	return true;
}

bool PS_Channel::GetOcpState()
{
	return OcpState.Val;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetOcpDelay(float ocpDelay)
{
	if (ocpDelay > OcpDelay.Max || ocpDelay < OcpDelay.Min) { return false; }

	if (OcpDelay.Val != ocpDelay)
	{
		OcpDelay.Val = ocpDelay;
		PSOcpDelayChanged(this);
	}
	return true;
}

float PS_Channel::GetOcpDelay()
{
	return OcpDelay.Val;
}

//----------------------------------------------------------------------------------------------------------

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

void PS_Channel::PSCurrentChanged(void* channel)
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

void PS_Channel::PSOvpLevelChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.DevSettingsNeedSaving = true;
}

void PS_Channel::PSOvpStateChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.DevSettingsNeedSaving = true;
}

void PS_Channel::PSOvpDelayChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.DevSettingsNeedSaving = true;
}

void PS_Channel::PSOcpLevelChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.DevSettingsNeedSaving = true;
}

void PS_Channel::PSOcpStateChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.DevSettingsNeedSaving = true;
}

void PS_Channel::PSOcpDelayChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.DevSettingsNeedSaving = true;
}