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
#include "../UserControlsIndicators/UserIndicatorEnum.cpp"
#include "../Channels/PS_Channel.h"
#include "Icons.h"

ScreenPS::ScreenPS() : ScreenBase("PS"),
	_ctrlPSVoltage(VOLTAGE_CONTROL_POSX, VOLTAGE_CONTROL_POSY, &Device.PsChannel.Amplitude.Val, "V", 0, Device.PsChannel.Amplitude.Min, Device.PsChannel.Amplitude.Max, &Device.PsChannel, &PS_Channel::PSAmplitudeChanged, icon_voltage_bits),
	_ctrlPSCurrent(CURRENT_CONTROL_POSX, CURRENT_CONTROL_POSY, &Device.PsChannel.Current.Val, "A", 0, Device.PsChannel.Current.Min, Device.PsChannel.Current.Max, &Device.PsChannel, &PS_Channel::PSCurrentChanged, icon_current_bits),
	_ctrlOutputEnable(OUTPUT_STATE_CONTROL_POSX, OUTPUT_STATE_CONTROL_POSY, &Device.PsChannel.Enabled.Val, &Device.PsChannel, &PS_Channel::PSEnabledChanged, icon_OnOff_bits),
	_indPSVoltage(INFO_TEXTS_POSX, INFO_TEXT_VOLTAGE_POSY, &Device.PsChannel.MeasuredAmplitude, "V"),
	_indPSCurrent(INFO_TEXTS_POSX, INFO_TEXT_CURRENT_POSY, &Device.PsChannel.MeasuredCurrent, "A"),
	_indPSPower(INFO_TEXTS_POSX, INFO_TEXT_POWER_POSY, &Device.PsChannel.MeasuredPower, "W"),
	_indPSState(INFO_TEXTS_POSX, INFO_TEXT_STATE_POSY, &Device.PsChannel.PsState, PSStatesNames, 5)
{
	_ctrlPSVoltage.IsSelected = true;
	
	_userControls[0] = &_ctrlPSVoltage;
	_userControls[1] = &_ctrlPSCurrent;
	_userControls[2] = &_ctrlOutputEnable;
	_numUserControls = 3;
	
	_userIndicators[0] = &_indPSVoltage;
	_userIndicators[1] = &_indPSCurrent;
	_userIndicators[2] = &_indPSPower;
	_userIndicators[3] = &_indPSState;
	_numUserIndicators = 4;
}

void ScreenPS::Draw(u8g_t* u8g, bool isFirstPage)
{
	u8g_DrawXBMP(u8g, u8g_GetWidth(u8g) - CONTROLS_ICON_SIZE - 2, u8g_GetHeight(u8g) - CONTROLS_ICON_SIZE - 2, CONTROLS_ICON_SIZE, CONTROLS_ICON_SIZE, icon_supplyDC_bits);
	
	ScreenBase::Draw(u8g, isFirstPage);
}