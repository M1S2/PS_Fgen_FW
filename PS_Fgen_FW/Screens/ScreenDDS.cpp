/*
 * ScreenDDS.cpp
 * Created: 07.01.2021 19:34:55
 * Author: Markus Scheich
 */ 

#include "../Device.h"

#ifdef DDS_SUBSYSTEM_ENABLED

ContainerListDefault list_DDS(40, 0, 240 - 40, 64);

void ScreenDDS1SignalFormChanged(void* channel);
void ScreenDDS2SignalFormChanged(void* channel);
void OnButtonConfigTTLPWM(void* context);

// ***** DDS1 pages *****
#define DDS1_COLUMN1_POSX		SCREEN_TAB_WIDTH + 5
#define DDS1_COLUMN2_POSX		DDS1_COLUMN1_POSX + 78
#define DDS1_COLUMN3_POSX		DDS1_COLUMN2_POSX + 35
#define DDS1_COLUMN4_POSX		DDS1_COLUMN3_POSX + 35
#define DDS1_ROW1_POSY			25
#define DDS1_ROW2_POSY			DDS1_ROW1_POSY + 20

ContainerPageDefault page_DDS1_1;
Icon ico_dds(SCREEN_TAB_WIDTH + 5, 3, icon_supplyAC_width, icon_supplyAC_height, icon_supplyAC_bits);
Label<5> lbl_DDS1_caption(SCREEN_TAB_WIDTH + 25, 5, "DDS1");

Icon ico_DDS_Frequency(DDS1_COLUMN1_POSX, DDS1_ROW1_POSY - 2, icon_frequency_width, icon_frequency_height, icon_frequency_bits);
NumericControl<float, 15> numCtrl_DDS1_Frequency(DDS1_COLUMN1_POSX + icon_frequency_width + 3, DDS1_ROW1_POSY, &Device.DdsChannel1.Frequency.Val, "Hz", DDS_MIN_FREQ, DDS_MAX_FREQ, 3, &Device.DdsChannel1, &DDS_Channel::DDSFrequencyChanged);
Icon ico_DDS_SignalForm(DDS1_COLUMN3_POSX, DDS1_ROW1_POSY - 2, icon_signalForm_width, icon_signalForm_height, icon_signalForm_bits);
EnumControl<SignalForms_t> enumCtrl_DDS1_SignalForm(DDS1_COLUMN3_POSX + icon_signalForm_width + 3, DDS1_ROW1_POSY, &Device.DdsChannel1.SignalForm.Val, SignalFormsNames, NUM_SIGNALFORM_ELEMENTS, &Device.DdsChannel1, &ScreenDDS1SignalFormChanged);
Icon ico_DDS_Amplitude(DDS1_COLUMN1_POSX, DDS1_ROW2_POSY - 2, icon_signalAmplitude_width, icon_signalAmplitude_height, icon_signalAmplitude_bits);
NumericControl<float> numCtrl_DDS1_Amplitude(DDS1_COLUMN1_POSX + icon_signalAmplitude_width + 3, DDS1_ROW2_POSY, &Device.DdsChannel1.Amplitude.Val, "Vpp", DDS_MIN_AMPLITUDE, DDS_MAX_AMPLITUDE, 2, &Device.DdsChannel1, &DDS_Channel::DDSAmplitudeChanged);
Icon ico_DDS_Offset(DDS1_COLUMN2_POSX, DDS1_ROW2_POSY - 2, icon_signalOffset_width, icon_signalOffset_height, icon_signalOffset_bits);
NumericControl<float> numCtrl_DDS1_Offset(DDS1_COLUMN2_POSX + icon_signalOffset_width + 3, DDS1_ROW2_POSY, &Device.DdsChannel1.Offset.Val, "V", DDS_MIN_OFFSET, DDS_MAX_OFFSET, 2, &Device.DdsChannel1, &DDS_Channel::DDSOffsetChanged);
Icon ico_DDS_Enabled(DDS1_COLUMN4_POSX, DDS1_ROW2_POSY, icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_DDS1_Enabled(DDS1_COLUMN4_POSX + icon_OnOff_width + 3, DDS1_ROW2_POSY, &Device.DdsChannel1.Enabled.Val, &Device.DdsChannel1, &DDS_Channel::DDSEnabledChanged);

ContainerPageDefault page_DDS1_2;
Icon ico_DDS_PWM(DDS1_COLUMN1_POSX, DDS1_ROW1_POSY - 2, icon_pwm_width, icon_pwm_height, icon_pwm_bits);
NumericControl<float> numCtrl_DDS1_PWMValue(DDS1_COLUMN1_POSX + icon_frequency_width + 3, DDS1_ROW1_POSY, &Device.DdsChannel1.PWM_Value.Val, "%%", 0, 100, 2, &Device.DdsChannel1, &DDS_Channel::DDSPWMValueChanged);	// Use two % signs because the sprintf function used in the NumericIndicator part is using one % sign as part of the format placeholders
ButtonControl<15> button_DDS1_Config5VPWM(DDS1_COLUMN3_POSX, DDS1_ROW1_POSY, 80, DEFAULT_UI_ELEMENT_HEIGHT, "Config TTL PWM", &Device.DdsChannel1, &OnButtonConfigTTLPWM);

// ***** DDS2 pages *****
#define DDS2_COLUMN1_POSX		DDS1_COLUMN1_POSX
#define DDS2_COLUMN2_POSX		DDS1_COLUMN2_POSX
#define DDS2_COLUMN3_POSX		DDS1_COLUMN3_POSX
#define DDS2_COLUMN4_POSX		DDS1_COLUMN4_POSX
#define DDS2_ROW1_POSY			DDS1_ROW1_POSY
#define DDS2_ROW2_POSY			DDS1_ROW2_POSY

ContainerPageDefault page_DDS2_1;
Label<5> lbl_DDS2_caption(SCREEN_TAB_WIDTH + 25, 5, "DDS2");

NumericControl<float, 15> numCtrl_DDS2_Frequency(DDS2_COLUMN1_POSX + icon_frequency_width + 3, DDS2_ROW1_POSY, &Device.DdsChannel2.Frequency.Val, "Hz", DDS_MIN_FREQ, DDS_MAX_FREQ, 3, &Device.DdsChannel2, &DDS_Channel::DDSFrequencyChanged);
EnumControl<SignalForms_t> enumCtrl_DDS2_SignalForm(DDS2_COLUMN3_POSX + icon_signalForm_width + 3, DDS2_ROW1_POSY, &Device.DdsChannel2.SignalForm.Val, SignalFormsNames, NUM_SIGNALFORM_ELEMENTS, &Device.DdsChannel2, &ScreenDDS2SignalFormChanged);
NumericControl<float> numCtrl_DDS2_Amplitude(DDS2_COLUMN1_POSX + icon_signalAmplitude_width + 3, DDS2_ROW2_POSY, &Device.DdsChannel2.Amplitude.Val, "Vpp", DDS_MIN_AMPLITUDE, DDS_MAX_AMPLITUDE, 2, &Device.DdsChannel2, &DDS_Channel::DDSAmplitudeChanged);
NumericControl<float> numCtrl_DDS2_Offset(DDS2_COLUMN2_POSX + icon_signalOffset_width + 3, DDS2_ROW2_POSY, &Device.DdsChannel2.Offset.Val, "V", DDS_MIN_OFFSET, DDS_MAX_OFFSET, 2, &Device.DdsChannel2, &DDS_Channel::DDSOffsetChanged);
BoolControl boolCtrl_DDS2_Enabled(DDS2_COLUMN4_POSX + icon_OnOff_width + 3, DDS2_ROW2_POSY, &Device.DdsChannel2.Enabled.Val, &Device.DdsChannel2, &DDS_Channel::DDSEnabledChanged);

ContainerPageDefault page_DDS2_2;
NumericControl<float> numCtrl_DDS2_PWMValue(DDS2_COLUMN1_POSX + icon_frequency_width + 3, DDS2_ROW1_POSY, &Device.DdsChannel2.PWM_Value.Val, "%%", 0, 100, 2, &Device.DdsChannel2, &DDS_Channel::DDSPWMValueChanged);	// Use two % signs because the sprintf function used in the NumericIndicator part is using one % sign as part of the format placeholders
ButtonControl<15> button_DDS2_Config5VPWM(DDS2_COLUMN3_POSX, DDS2_ROW1_POSY, 80, DEFAULT_UI_ELEMENT_HEIGHT, "Config TTL PWM", &Device.DdsChannel2, &OnButtonConfigTTLPWM);

void ScreenDDS1SignalFormChanged(void* channel)
{	
	// Show the second DDS page if the channel is a PWM channel
	if (((DDS_Channel*)channel)->GetSignalForm() == PWM)
	{
		page_DDS1_2.Visible = true;
	}
	else
	{
		page_DDS1_2.Visible = false;
	}	
	DDS_Channel::DDSSignalFormChanged(channel);
}

void ScreenDDS2SignalFormChanged(void* channel)
{
	// Show the second DDS page if the channel is a PWM channel
	if (((DDS_Channel*)channel)->GetSignalForm() == PWM)
	{
		page_DDS2_2.Visible = true;
	}
	else
	{
		page_DDS2_2.Visible = false;
	}	
	DDS_Channel::DDSSignalFormChanged(channel);
}

void ScreenDDSUpdateVisibility()
{
	ScreenDDS1SignalFormChanged(&Device.DdsChannel1);
	ScreenDDS2SignalFormChanged(&Device.DdsChannel2);
}

void OnButtonConfigTTLPWM(void* context)
{
	if(context == NULL) { return; }
	DDS_Channel* channel = (DDS_Channel*)context;
	// Set the amplitude and offset to achieve a low level of 0V and a high level of 5V
	channel->SetAmplitude(5.0f);
	channel->SetOffset(2.5f);
}

UIElement* uiBuildScreenDDS()
{
	numCtrl_DDS1_Frequency.Width = 89;
	numCtrl_DDS1_Amplitude.Width = 53;
	numCtrl_DDS1_Offset.Width = 42;
	numCtrl_DDS1_Amplitude.CurrentDigitPosition = -1;	// select the 0.1 V digit.
	numCtrl_DDS1_Offset.CurrentDigitPosition = -1;		// select the 0.1 V digit.
	
	page_DDS1_1.AddItem(&ico_dds);
	page_DDS1_1.AddItem(&lbl_DDS1_caption);
	page_DDS1_1.AddItem(&ico_DDS_Frequency);
	page_DDS1_1.AddItem(&numCtrl_DDS1_Frequency);
	page_DDS1_1.AddItem(&ico_DDS_SignalForm);
	page_DDS1_1.AddItem(&enumCtrl_DDS1_SignalForm);
	page_DDS1_1.AddItem(&ico_DDS_Amplitude);
	page_DDS1_1.AddItem(&numCtrl_DDS1_Amplitude);
	page_DDS1_1.AddItem(&ico_DDS_Offset);
	page_DDS1_1.AddItem(&numCtrl_DDS1_Offset);
	page_DDS1_1.AddItem(&ico_DDS_Enabled);
	page_DDS1_1.AddItem(&boolCtrl_DDS1_Enabled);
	page_DDS1_1.InitItems();
	
	page_DDS1_2.AddItem(&ico_dds);
	page_DDS1_2.AddItem(&lbl_DDS1_caption);
	page_DDS1_2.AddItem(&ico_DDS_PWM);
	page_DDS1_2.AddItem(&numCtrl_DDS1_PWMValue);
	page_DDS1_2.AddItem(&button_DDS1_Config5VPWM);
	page_DDS1_2.InitItems();
	
	numCtrl_DDS2_Frequency.Width = 89;
	numCtrl_DDS2_Amplitude.Width = 53;
	numCtrl_DDS2_Offset.Width = 42;
	numCtrl_DDS2_Amplitude.CurrentDigitPosition = -1;	// select the 0.1 V digit.
	numCtrl_DDS2_Offset.CurrentDigitPosition = -1;		// select the 0.1 V digit.
	
	page_DDS2_1.AddItem(&ico_dds);
	page_DDS2_1.AddItem(&lbl_DDS2_caption);
	page_DDS2_1.AddItem(&ico_DDS_Frequency);
	page_DDS2_1.AddItem(&numCtrl_DDS2_Frequency);
	page_DDS2_1.AddItem(&ico_DDS_SignalForm);
	page_DDS2_1.AddItem(&enumCtrl_DDS2_SignalForm);
	page_DDS2_1.AddItem(&ico_DDS_Amplitude);
	page_DDS2_1.AddItem(&numCtrl_DDS2_Amplitude);
	page_DDS2_1.AddItem(&ico_DDS_Offset);
	page_DDS2_1.AddItem(&numCtrl_DDS2_Offset);
	page_DDS2_1.AddItem(&ico_DDS_Enabled);
	page_DDS2_1.AddItem(&boolCtrl_DDS2_Enabled);
	page_DDS2_1.InitItems();
	
	page_DDS2_2.AddItem(&ico_dds);
	page_DDS2_2.AddItem(&lbl_DDS2_caption);
	page_DDS2_2.AddItem(&ico_DDS_PWM);
	page_DDS2_2.AddItem(&numCtrl_DDS2_PWMValue);
	page_DDS2_2.AddItem(&button_DDS2_Config5VPWM);
	page_DDS2_2.InitItems();
	
	list_DDS.AddItem(&page_DDS1_1);
	list_DDS.AddItem(&page_DDS1_2);
	list_DDS.AddItem(&page_DDS2_1);
	list_DDS.AddItem(&page_DDS2_2);
	
	return &list_DDS;
}

#endif /* DDS_SUBSYSTEM_ENABLED */