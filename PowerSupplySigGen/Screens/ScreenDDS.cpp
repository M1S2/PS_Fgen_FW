/*
 * ScreenDDS.cpp
 *
 * Created: 07.01.2021 19:34:55
 *  Author: V17
 */ 

#include "ScreenDDS.h"
#include "../UserControlsIndicators/UserControlNumeric.cpp"
#include "../UserControlsIndicators/UserControlEnum.cpp"

ScreenDDS::ScreenDDS() : ScreenBase("DDS1"),
	_ctrlDDSSignalForm(SIGNALFORM_CONTROL_POSX, SIGNALFORM_CONTROL_POSY, &DDS_Channel1.SignalForm, SignalFormsNames, 4, &DDSUpdateSignalFormsCallbackFunction),
	_ctrlDDSFrequency(FREQUENCY_CONTROL_POSX, FREQUENCY_CONTROL_POSY, &DDS_Channel1.Frequency, "Hz", 0, 1, 100000.0f, &DDSUpdateIncrementsCallbackFunction),
	_ctrlDDSEnabled(ENABLED_CONTROL_POSX, ENABLED_CONTROL_POSY, &DDS_Channel1.Enabled, &DDSUpdateEnabledCallbackFunction),
	_ctrlDDSAmplitude(AMPLITUDE_CONTROL_POSX, AMPLITUDE_CONTROL_POSY, &DDS_Channel1.Amplitude, "Vpp", 0, 0, DDS_AMPLITUDE_MAX, &DDSUpdateWaveTableCallbackFunction),
	_ctrlDDSOffset(OFFSET_CONTROL_POSX, OFFSET_CONTROL_POSY, &DDS_Channel1.Offset, "V", 0, -DDS_AMPLITUDE_MAX, DDS_AMPLITUDE_MAX, &DDSUpdateWaveTableCallbackFunction)
{
	_ctrlDDSFrequency.IsSelected = true;
	
	_userControls[0] = &_ctrlDDSSignalForm;
	_userControls[1] = &_ctrlDDSFrequency;
	_userControls[2] = &_ctrlDDSEnabled;
	_userControls[3] = &_ctrlDDSAmplitude;
	_userControls[4] = &_ctrlDDSOffset;
	_numUserControls = 5;
}

void ScreenDDS::Draw(u8g_t* u8g, bool isFirstPage)
{
	ScreenBase::Draw(u8g, isFirstPage);
}