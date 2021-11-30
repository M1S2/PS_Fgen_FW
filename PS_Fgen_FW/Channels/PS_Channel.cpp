/*
 * PS_Channel.cpp
 * Created: 23.09.2020 19:47:47
 * Author: Markus Scheich
 */ 

#include "PS_Channel.h"
#include "../Device.h"

const char* PsRegulationModesNames[] = { "FIX", "CV", "CV&CC" };


#ifdef PS_SUBSYSTEM_ENABLED

const char* PsStatesNames[] = { "CV", "CC", "OVL", "OVP", "OCP", "OPP" };

PS_Channel::PS_Channel(float minVolt, float maxVolt, float minCurrent, float maxCurrent, uint8_t minOvpLevel, uint8_t maxOvpLevel, float minOvpDelay, float maxOvpDelay, uint8_t minOcpLevel, uint8_t maxOcpLevel, float minOcpDelay, float maxOcpDelay, float minOppLevel, float maxOppLevel, float minOppDelay, float maxOppDelay) : Channel(POWER_SUPPLY_CHANNEL_TYPE)
{
	Enabled = Parameter<bool>(false, false, true, false, true);
	Voltage = Parameter<float>(0, minVolt, maxVolt, 5, 1);
	Current = Parameter<float>(0, minCurrent, maxCurrent, 0.1, 0.1);
	
	OvpState = Parameter<bool>(false, false, true, false, true);
	OvpLevel = Parameter<uint8_t>(0, minOvpLevel, maxOvpLevel, 120, 1);
	OvpDelay = Parameter<float>(0.1, minOvpDelay, maxOvpDelay, 0.1, 0.1);
	
	OcpState = Parameter<bool>(false, false, true, false, true);
	OcpLevel = Parameter<uint8_t>(0, minOcpLevel, maxOcpLevel, 120, 1);
	OcpDelay = Parameter<float>(0.1, minOcpDelay, maxOcpDelay, 0.1, 0.1);
	
	OppState = Parameter<bool>(false, false, true, false, true);
	OppLevel = Parameter<float>(0, minOppLevel, maxOppLevel, 1, 0.1);
	OppDelay = Parameter<float>(0.1, minOppDelay, maxOppDelay, 0.1, 0.1);
	
	TimeCounter_OvpDelay_ms = 0;
	TimeCounter_OcpDelay_ms = 0;
	TimeCounter_OppDelay_ms = 0;
	PsState = PS_STATE_CV;
	RegulationMode = PS_REG_MODE_CV_CC;
	_setVoltage = 0;
	_PIDVoltErrorSum = 0;
	_PIDVoltErrorLast = 0;	
	_PIDCurrentErrorSum = 0;
	_PIDCurrentErrorLast = 0;
	MeasuredLoadResistance = 1;
}

void PS_Channel::SwitchOffOutput()
{
	MCP4921_Voltage_Set(0);
}
		
void PS_Channel::UpdateOutput()
{
	if(GetEnabled() && (PsState == PS_STATE_CV || PsState == PS_STATE_CC || PsState == PS_STATE_OVL))
	{
		MCP4921_Voltage_Set(_setVoltage / 2);		// divided by two because of OpAmp in circuit that has an amplification of 2
	}
	else
	{
		SwitchOffOutput();
	}
}

void PS_Channel::DoRegulationISR()
{
	if(GetEnabled() && (PsState == PS_STATE_CV || PsState == PS_STATE_CC || PsState == PS_STATE_OVL))
	{		
		MeasuredPower = MeasuredVoltage * MeasuredCurrent;

		MeasuredLoadResistance = (MeasuredCurrent < 0.001f ? MeasuredLoadResistance : (MeasuredVoltage / MeasuredCurrent));
		
		if(RegulationMode == PS_REG_MODE_FIX)
		{
			_setVoltage = GetVoltage();
			_setVoltage /= (Device.CalibrationFactors.Cal_PS_VOLT == 0) ? 1 : Device.CalibrationFactors.Cal_PS_VOLT;
		}
		else
		{
			/********************************************************
			 * Voltage PID regulator 
			 * see: https://rn-wissen.de/wiki/index.php/Regelungstechnik 
			 ********************************************************/
			float PIDVoltError = GetVoltage() - MeasuredVoltage;			// U_target - U_measured
			float tmpPIDVoltErrorSum = _PIDVoltErrorSum + PIDVoltError;
			_setVoltage = PS_VOLT_PID_P * PIDVoltError + PS_VOLT_PID_I * (POWER_SUPPLY_REG_INTERVAL_MS / 1000.0f) * tmpPIDVoltErrorSum + (PS_VOLT_PID_D / (POWER_SUPPLY_REG_INTERVAL_MS / 1000.0f)) * (PIDVoltError - _PIDVoltErrorLast);
			
			/********************************************************
			 * Current PID regulator 
			 ********************************************************/
			float PIDCurrentError = GetCurrent() - MeasuredCurrent;			// I_target - I_measured
			float tmpPIDCurrentErrorSum = _PIDCurrentErrorSum + PIDCurrentError;
			float setCurrent = PS_CURRENT_PID_P * PIDCurrentError + PS_CURRENT_PID_I * (POWER_SUPPLY_REG_INTERVAL_MS / 1000.0f) * tmpPIDCurrentErrorSum + (PS_CURRENT_PID_D / (POWER_SUPPLY_REG_INTERVAL_MS / 1000.0f)) * (PIDCurrentError - _PIDCurrentErrorLast);
			if(setCurrent < 0) { setCurrent = 0; }
			float setVoltageForCurrent = setCurrent * MeasuredLoadResistance;
			
			/********************************************************
			 * Decide between ConstantCurrent (CC) or ConstantVoltage (CV) mode
			 * This is only supported if the Regulation Mode is set to CV&CC
			 ********************************************************/
			if(fabs(setVoltageForCurrent) < fabs(_setVoltage) && RegulationMode == PS_REG_MODE_CV_CC)
			{
				PsState = PS_STATE_CC;
				_setVoltage = setVoltageForCurrent;
				_PIDCurrentErrorLast = PIDCurrentError;
			}
			else
			{
				PsState = PS_STATE_CV;
				_PIDVoltErrorLast = PIDVoltError;
			}
		
			/********************************************************
			 * PID Integrator anti-windup
			 * see: https://www.embeddedrelated.com/showcode/346.php
			 ********************************************************/
			if (_setVoltage > PS_MAX_VOLTAGE)							// Positive saturation? Output is not regulated (Open-loop).
			{
				_setVoltage = PS_MAX_VOLTAGE;							// Clamp the output
				if (PsState == PS_STATE_CV && PIDVoltError < 0)			// Error is the opposite sign? Update integration error.
				{
					_PIDVoltErrorSum = tmpPIDVoltErrorSum;
				}
				else if (PsState == PS_STATE_CC && PIDCurrentError < 0)	// Error is the opposite sign? Update integration error.
				{
					_PIDCurrentErrorSum = tmpPIDCurrentErrorSum;
				}
				PsState = PS_STATE_OVL;									// Set the power supply state to overload (because the output is saturated).
			}
			else if (_setVoltage < 0)									// Negative saturation? Output is not regulated (Open-loop).
			{
				_setVoltage = 0;										// Clamp the output
				if (PsState == PS_STATE_CV && PIDVoltError > 0)			// Error is the opposite sign? Update integration error.
				{
					_PIDVoltErrorSum = tmpPIDVoltErrorSum;
				}
				else if (PsState == PS_STATE_CC && PIDCurrentError > 0)	// Error is the opposite sign? Update integration error.
				{
					_PIDCurrentErrorSum = tmpPIDCurrentErrorSum;
				}
				PsState = PS_STATE_OVL;									// Set the power supply state to overload (because the output is saturated).
			}
			else														// Output is regulated (Closed-loop).
			{
				if (PsState == PS_STATE_CV)
				{
					_PIDVoltErrorSum = tmpPIDVoltErrorSum;
				}
				else if (PsState == PS_STATE_CC)
				{
					_PIDCurrentErrorSum = tmpPIDCurrentErrorSum;
				}
			}
		}
	
		CheckProtections();
		UpdateOutput();
	}
	else
	{
		// Reset the PID error counters if the output is disabled or a protection is active
		_PIDVoltErrorSum = 0;
		_PIDVoltErrorLast = 0;
		_PIDCurrentErrorSum = 0;
		_PIDCurrentErrorLast = 0;
		
		// Set the MeasuredPower to 0 if the output is disabled or a protection is active
		MeasuredPower = 0;
	}
}

void PS_Channel::CheckProtections()
{
	if(GetOvpState() && MeasuredVoltage > (GetVoltage() * (GetOvpLevel() / 100.0f)))
	{
		TimeCounter_OvpDelay_ms += POWER_SUPPLY_REG_INTERVAL_MS;
	}
	else { TimeCounter_OvpDelay_ms = 0; }
	
	if(GetOcpState() && MeasuredCurrent > (GetCurrent() * (GetOcpLevel() / 100.0f)))
	{
		TimeCounter_OcpDelay_ms += POWER_SUPPLY_REG_INTERVAL_MS;
	}
	else { TimeCounter_OcpDelay_ms = 0; }

	if(GetOppState() && MeasuredPower > GetOppLevel())
	{
		TimeCounter_OppDelay_ms += POWER_SUPPLY_REG_INTERVAL_MS;
	}
	else { TimeCounter_OppDelay_ms = 0; }

	if(TimeCounter_OvpDelay_ms >= (1000 * GetOvpDelay()))
	{
		PsState = PS_STATE_OVP;
		TimeCounter_OvpDelay_ms = 0;
	}
	else if(TimeCounter_OcpDelay_ms >= (1000 * GetOcpDelay()))
	{
		PsState = PS_STATE_OCP;
		TimeCounter_OcpDelay_ms = 0;
	}
	else if(TimeCounter_OppDelay_ms >= (1000 * GetOppDelay()))
	{
		PsState = PS_STATE_OPP;
		TimeCounter_OppDelay_ms = 0;
	}
}

void PS_Channel::ClearProtections()
{
	if(PsState == PS_STATE_OVP)
	{
		PsState = PS_STATE_CV;
		TimeCounter_OvpDelay_ms = 0;
		UpdateOutput();
	}
	else if(PsState == PS_STATE_OCP)
	{
		PsState = PS_STATE_CV;
		TimeCounter_OcpDelay_ms = 0;
		UpdateOutput();
	}
	else if(PsState == PS_STATE_OPP)
	{
		PsState = PS_STATE_CV;
		TimeCounter_OppDelay_ms = 0;
		UpdateOutput();
	}
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetRegulationMode(PsRegulationModes_t regulationMode)
{
	if (RegulationMode != regulationMode)
	{
		RegulationMode = regulationMode;
		PSRegulationModeChanged(this);
	}
	return true;
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

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetVoltage(float voltage)
{
	if (voltage > Voltage.Max || voltage < Voltage.Min) { return false; }

	if (Voltage.Val != voltage)
	{
		Voltage.Val = voltage;
		PSVoltageChanged(this);
	}
	return true;
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

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetOppLevel(float oppLevel)
{
	if (oppLevel > OppLevel.Max || oppLevel < OppLevel.Min) { return false; }

	if (OppLevel.Val != oppLevel)
	{
		OppLevel.Val = oppLevel;
		PSOppLevelChanged(this);
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetOppState(bool oppState)
{
	if (oppState > OppState.Max || oppState < OppState.Min) { return false; }

	if (OppState.Val != oppState)
	{
		OppState.Val = oppState;
		PSOppStateChanged(this);
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------

bool PS_Channel::SetOppDelay(float oppDelay)
{
	if (oppDelay > OppDelay.Max || oppDelay < OppDelay.Min) { return false; }

	if (OppDelay.Val != oppDelay)
	{
		OppDelay.Val = oppDelay;
		PSOppDelayChanged(this);
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------

void PS_Channel::PSRegulationModeChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	PS_Channel* psChannel = (PS_Channel*)channel;
	psChannel->UpdateOutput();
	Device.SetSettingsChanged(true);		
}

void PS_Channel::PSEnabledChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	PS_Channel* psChannel = (PS_Channel*)channel;
	psChannel->UpdateOutput();
	Device.SetSettingsChanged(true);
}

void PS_Channel::PSVoltageChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	PS_Channel* psChannel = (PS_Channel*)channel;
	psChannel->UpdateOutput();
	Device.SetSettingsChanged(true);
}

void PS_Channel::PSCurrentChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	PS_Channel* psChannel = (PS_Channel*)channel;
	psChannel->UpdateOutput();
	Device.SetSettingsChanged(true);
}

void PS_Channel::PSOvpLevelChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.SetSettingsChanged(true);
}

void PS_Channel::PSOvpStateChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.SetSettingsChanged(true);
}

void PS_Channel::PSOvpDelayChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.SetSettingsChanged(true);
}

void PS_Channel::PSOcpLevelChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.SetSettingsChanged(true);
}

void PS_Channel::PSOcpStateChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.SetSettingsChanged(true);
}

void PS_Channel::PSOcpDelayChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.SetSettingsChanged(true);
}

void PS_Channel::PSOppLevelChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.SetSettingsChanged(true);
}

void PS_Channel::PSOppStateChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.SetSettingsChanged(true);
}

void PS_Channel::PSOppDelayChanged(void* channel)
{
	if (((Channel*)channel)->GetChannelType() != POWER_SUPPLY_CHANNEL_TYPE) { return; }
	/* Parameter only used in DeviceTimerTickISR() */
	Device.SetSettingsChanged(true);
}

#endif /* PS_SUBSYSTEM_ENABLED */