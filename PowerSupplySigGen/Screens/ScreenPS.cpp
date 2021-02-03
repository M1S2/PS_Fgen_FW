/*
 * ScreenPS.cpp
 *
 * Created: 07.11.2020 13:09:35
 *  Author: V17
 */ 

#include "ScreenPS.h"
#include "../Device.h"
#include "../UserControlsIndicators/UserControlNumeric.cpp"
#include "../UserControlsIndicators/UserIndicatorNumeric.cpp"
#include "../Outputs/PS_Channel.h"

ScreenPS::ScreenPS() : ScreenBase("PS"),
	_ctrlPSVoltage(VOLTAGE_CONTROL_POSX, VOLTAGE_CONTROL_POSY, &Device.Channels[0].Amplitude.Val, "V", 0, Device.Channels[0].Amplitude.Min, Device.Channels[0].Amplitude.Max, &Device.Channels[0], &PS_Channel::PSAmplitudeChanged),
	_ctrlLoadImpedance(LOAD_IMPEDANCE_CONTROL_POSX, LOAD_IMPEDANCE_CONTROL_POSY, &Device.Channels[0].LoadImpedance.Val, "Ohm", 0, Device.Channels[0].LoadImpedance.Min, Device.Channels[0].LoadImpedance.Max, &Device.Channels[0], &PS_Channel::PSLoadImpedanceChanged),
	_ctrlOutputEnable(OUTPUT_STATE_CONTROL_POSX, OUTPUT_STATE_CONTROL_POSY, &Device.Channels[0].Enabled.Val, &Device.Channels[0], &PS_Channel::PSEnabledChanged),
	_indPSVoltage(INFO_TEXTS_POSX, INFO_TEXT_VOLTAGE_POSY, &Device.DeviceVoltages.PS_VOLT, "V"),
	_indPSCurrent(INFO_TEXTS_POSX, INFO_TEXT_CURRENT_POSY, &Device.DeviceVoltages.PS_CURR, "A"),
	_indPSPower(INFO_TEXTS_POSX, INFO_TEXT_POWER_POSY, &Device.DeviceVoltages.PS_POWER, "W")
{
	_ctrlPSVoltage.IsSelected = true;
	
	_userControls[0] = &_ctrlPSVoltage;
	_userControls[1] = &_ctrlLoadImpedance;
	_userControls[2] = &_ctrlOutputEnable;
	_numUserControls = 3;
	
	_userIndicators[0] = &_indPSVoltage;
	_userIndicators[1] = &_indPSCurrent;
	_userIndicators[2] = &_indPSPower;
	_numUserIndicators = 3;
}

void ScreenPS::Draw(u8g_t* u8g, bool isFirstPage)
{
	ScreenBase::Draw(u8g, isFirstPage);
}