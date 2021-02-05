/*
 * ScreenDDS.cpp
 *
 * Created: 07.01.2021 19:34:55
 *  Author: V17
 */ 

#include "ScreenDDS.h"
#include "../Device.h"
#include "../UserControlsIndicators/UserControlNumeric.cpp"
#include "../UserControlsIndicators/UserControlEnum.cpp"
#include "../Outputs/DDS_Channel.h"

ScreenDDS::ScreenDDS() : ScreenBase("DDS1"),
	_ctrlDDSSignalForm(SIGNALFORM_CONTROL_POSX, SIGNALFORM_CONTROL_POSY, &Device.DdsChannel1.SignalForm.Val, SignalFormsNames, 4, &Device.DdsChannel1, &DDS_Channel::DDSSignalFormChanged),
	_ctrlDDSFrequency(FREQUENCY_CONTROL_POSX, FREQUENCY_CONTROL_POSY, &Device.DdsChannel1.Frequency.Val, "Hz", 0, Device.DdsChannel1.Frequency.Min, Device.DdsChannel1.Frequency.Max, &Device.DdsChannel1, &DDS_Channel::DDSFrequencyChanged),
	_ctrlDDSEnabled(ENABLED_CONTROL_POSX, ENABLED_CONTROL_POSY, &Device.DdsChannel1.Enabled.Val, &Device.DdsChannel1, &DDS_Channel::DDSEnabledChanged),
	_ctrlDDSAmplitude(AMPLITUDE_CONTROL_POSX, AMPLITUDE_CONTROL_POSY, &Device.DdsChannel1.Amplitude.Val, "Vpp", 0, Device.DdsChannel1.Amplitude.Min, Device.DdsChannel1.Amplitude.Max, &Device.DdsChannel1, &DDS_Channel::DDSAmplitudeChanged),
	_ctrlDDSOffset(OFFSET_CONTROL_POSX, OFFSET_CONTROL_POSY, &Device.DdsChannel1.Offset.Val, "V", 0, Device.DdsChannel1.Offset.Min, Device.DdsChannel1.Offset.Max, &Device.DdsChannel1, &DDS_Channel::DDSOffsetChanged)
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