/*
 * ScreenDDS.cpp
 *
 * Created: 07.01.2021 19:34:55
 *  Author: V17
 */ 

//#include "ScreenDDS.h"
#include "../Device.h"
/*#include "../UserControlsIndicators/UserControlNumeric.cpp"
#include "../UserControlsIndicators/UserControlEnum.cpp"
#include "../Channels/DDS_Channel.h"
#include "Icons.h"*/

ContainerList list_DDS(40, 0, 240 - 40, 64);

// ***** DDS1 page *****
ContainerPage page_DDS1;
Icon ico_dds(40, 3, icon_supplyAC_width, icon_supplyAC_height, icon_supplyAC_bits);
Label lbl_DDS1_caption(60, 5, "DDS1");

// ***** DDS2 page *****
ContainerPage page_DDS2;
Label lbl_DDS2_caption(60, 5, "DDS2");

UIElement* uiBuildScreenDDS()
{
	page_DDS1.AddItem(&ico_dds);
	page_DDS1.AddItem(&lbl_DDS1_caption);
	page_DDS1.InitItems();
	
	page_DDS2.AddItem(&ico_dds);
	page_DDS2.AddItem(&lbl_DDS2_caption);
	page_DDS2.InitItems();
	
	list_DDS.AddItem(&page_DDS1);
	list_DDS.AddItem(&page_DDS2);
	
	return &list_DDS;
}

/*ScreenDDS::ScreenDDS() : ScreenBase("DDS1"),
	_ctrlDDSSignalForm(SIGNALFORM_CONTROL_POSX, SIGNALFORM_CONTROL_POSY, &Device.DdsChannel1.SignalForm.Val, SignalFormsNames, 4, &Device.DdsChannel1, &DDS_Channel::DDSSignalFormChanged, icon_signalForm_bits),
	_ctrlDDSFrequency(FREQUENCY_CONTROL_POSX, FREQUENCY_CONTROL_POSY, &Device.DdsChannel1.Frequency.Val, "Hz", 0, Device.DdsChannel1.Frequency.Min, Device.DdsChannel1.Frequency.Max, &Device.DdsChannel1, &DDS_Channel::DDSFrequencyChanged, icon_frequency_bits),
	_ctrlDDSEnabled(ENABLED_CONTROL_POSX, ENABLED_CONTROL_POSY, &Device.DdsChannel1.Enabled.Val, &Device.DdsChannel1, &DDS_Channel::DDSEnabledChanged, icon_OnOff_bits),
	_ctrlDDSAmplitude(AMPLITUDE_CONTROL_POSX, AMPLITUDE_CONTROL_POSY, &Device.DdsChannel1.Amplitude.Val, "Vpp", 0, Device.DdsChannel1.Amplitude.Min, Device.DdsChannel1.Amplitude.Max, &Device.DdsChannel1, &DDS_Channel::DDSAmplitudeChanged, icon_signalAmplitude_bits),
	_ctrlDDSOffset(OFFSET_CONTROL_POSX, OFFSET_CONTROL_POSY, &Device.DdsChannel1.Offset.Val, "V", 0, Device.DdsChannel1.Offset.Min, Device.DdsChannel1.Offset.Max, &Device.DdsChannel1, &DDS_Channel::DDSOffsetChanged, icon_signalOffset_bits)
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
	u8g_DrawXBMP(u8g, u8g_GetWidth(u8g) - CONTROLS_ICON_SIZE - 2, u8g_GetHeight(u8g) - CONTROLS_ICON_SIZE - 2, CONTROLS_ICON_SIZE, CONTROLS_ICON_SIZE, icon_supplyAC_bits);
	
	ScreenBase::Draw(u8g, isFirstPage);
}*/