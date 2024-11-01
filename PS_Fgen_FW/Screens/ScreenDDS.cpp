/*
 * ScreenDDS.cpp
 * Created: 07.01.2021 19:34:55
 * Author: Markus Scheich
 */ 

#include "../Device.h"

#ifdef DDS_SUBSYSTEM_ENABLED

ContainerList list_DDS;

void ScreenDDS1SignalFormChanged(void* channel);
void ScreenDDS2SignalFormChanged(void* channel);
void OnButtonConfigTTLPWM(void* context);

// ***** DDS1 pages *****
ContainerGrid grid_DDS1(15, 6, 4, false, true);
Icon ico_DDS1(icon_supplyAC_width, icon_supplyAC_height, icon_supplyAC_bits, COLOR_FOREGROUND_HEADERS);
Label lbl_DDS1_caption("DDS1", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 5);

Icon ico_DDS1_Frequency(icon_frequency_width, icon_frequency_height, icon_frequency_bits);
NumericControl<float> numCtrl_DDS1_Frequency(&Device.DdsChannel1.Frequency.Val, "Hz", DDS_MIN_FREQ, DDS_MAX_FREQ, 3, &Device.DdsChannel1, &DDS_Channel::DDSFrequencyChanged);
Icon ico_DDS1_SignalForm(icon_signalForm_width, icon_signalForm_height, icon_signalForm_bits);
EnumControl<SignalForms_t> enumCtrl_DDS1_SignalForm(&Device.DdsChannel1.SignalForm.Val, SignalFormsNames, NUM_SIGNALFORM_ELEMENTS, &Device.DdsChannel1, &ScreenDDS1SignalFormChanged);
Icon ico_DDS1_Amplitude(icon_signalAmplitude_width, icon_signalAmplitude_height, icon_signalAmplitude_bits);
NumericControl<float> numCtrl_DDS1_Amplitude(&Device.DdsChannel1.Amplitude.Val, "Vpp", DDS_MIN_AMPLITUDE, DDS_MAX_AMPLITUDE, 2, &Device.DdsChannel1, &DDS_Channel::DDSAmplitudeChanged);
Icon ico_DDS1_Offset(icon_signalOffset_width, icon_signalOffset_height, icon_signalOffset_bits);
NumericControl<float> numCtrl_DDS1_Offset(&Device.DdsChannel1.Offset.Val, "V", DDS_MIN_OFFSET, DDS_MAX_OFFSET, 2, &Device.DdsChannel1, &DDS_Channel::DDSOffsetChanged);
Icon ico_DDS1_Enabled(icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_DDS1_Enabled(&Device.DdsChannel1.Enabled.Val, &Device.DdsChannel1, &DDS_Channel::DDSEnabledChanged);

Icon ico_DDS1_PWM(icon_pwm_width, icon_pwm_height, icon_pwm_bits);
NumericControl<float> numCtrl_DDS1_PWMValue(&Device.DdsChannel1.PWM_Value.Val, "%%", 0, 100, 2, &Device.DdsChannel1, &DDS_Channel::DDSPWMValueChanged);	// Use two % signs because the sprintf function used in the NumericIndicator part is using one % sign as part of the format placeholders
ButtonControl button_DDS1_Config5VPWM("Set TTL PWM", &Device.DdsChannel1, &OnButtonConfigTTLPWM, 15);

// ***** DDS2 pages *****
ContainerGrid grid_DDS2(15, 6, 4, false, true);
Icon ico_DDS2(icon_supplyAC_width, icon_supplyAC_height, icon_supplyAC_bits, COLOR_FOREGROUND_HEADERS);
Label lbl_DDS2_caption("DDS2", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 5);

Icon ico_DDS2_Frequency(icon_frequency_width, icon_frequency_height, icon_frequency_bits);
NumericControl<float> numCtrl_DDS2_Frequency(&Device.DdsChannel2.Frequency.Val, "Hz", DDS_MIN_FREQ, DDS_MAX_FREQ, 3, &Device.DdsChannel2, &DDS_Channel::DDSFrequencyChanged);
Icon ico_DDS2_SignalForm(icon_signalForm_width, icon_signalForm_height, icon_signalForm_bits);
EnumControl<SignalForms_t> enumCtrl_DDS2_SignalForm(&Device.DdsChannel2.SignalForm.Val, SignalFormsNames, NUM_SIGNALFORM_ELEMENTS, &Device.DdsChannel2, &ScreenDDS2SignalFormChanged);
Icon ico_DDS2_Amplitude(icon_signalAmplitude_width, icon_signalAmplitude_height, icon_signalAmplitude_bits);
NumericControl<float> numCtrl_DDS2_Amplitude(&Device.DdsChannel2.Amplitude.Val, "Vpp", DDS_MIN_AMPLITUDE, DDS_MAX_AMPLITUDE, 2, &Device.DdsChannel2, &DDS_Channel::DDSAmplitudeChanged);
Icon ico_DDS2_Offset(icon_signalOffset_width, icon_signalOffset_height, icon_signalOffset_bits);
NumericControl<float> numCtrl_DDS2_Offset(&Device.DdsChannel2.Offset.Val, "V", DDS_MIN_OFFSET, DDS_MAX_OFFSET, 2, &Device.DdsChannel2, &DDS_Channel::DDSOffsetChanged);
Icon ico_DDS2_Enabled(icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_DDS2_Enabled(&Device.DdsChannel2.Enabled.Val, &Device.DdsChannel2, &DDS_Channel::DDSEnabledChanged);

Icon ico_DDS2_PWM(icon_pwm_width, icon_pwm_height, icon_pwm_bits);
NumericControl<float> numCtrl_DDS2_PWMValue(&Device.DdsChannel2.PWM_Value.Val, "%%", 0, 100, 2, &Device.DdsChannel2, &DDS_Channel::DDSPWMValueChanged);	// Use two % signs because the sprintf function used in the NumericIndicator part is using one % sign as part of the format placeholders
ButtonControl button_DDS2_Config5VPWM("Set TTL PWM", &Device.DdsChannel2, &OnButtonConfigTTLPWM, 15);

void ScreenDDS1SignalFormChanged(void* channel)
{	
	// Show the second DDS page if the channel is a PWM channel
	if (((DDS_Channel*)channel)->GetSignalForm() == PWM)
	{
		ico_DDS1_PWM.Visible = true;
		numCtrl_DDS1_PWMValue.Visible = true;
		button_DDS1_Config5VPWM.Visible = true;
	}
	else
	{
		ico_DDS1_PWM.Visible = false;
		numCtrl_DDS1_PWMValue.Visible = false;
		button_DDS1_Config5VPWM.Visible = false;
	}	
	DDS_Channel::DDSSignalFormChanged(channel);
}

void ScreenDDS2SignalFormChanged(void* channel)
{
	// Show the second DDS page if the channel is a PWM channel
	if (((DDS_Channel*)channel)->GetSignalForm() == PWM)
	{
		ico_DDS2_PWM.Visible = true;
		numCtrl_DDS2_PWMValue.Visible = true;
		button_DDS2_Config5VPWM.Visible = true;
	}
	else
	{
		ico_DDS2_PWM.Visible = false;
		numCtrl_DDS2_PWMValue.Visible = false;
		button_DDS2_Config5VPWM.Visible = false;
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
	list_DDS.AddItem(&grid_DDS1);
	list_DDS.AddItem(&grid_DDS2);

	numCtrl_DDS1_Amplitude.CurrentDigitPosition = -1;	// select the 0.1 V digit.
	numCtrl_DDS1_Offset.CurrentDigitPosition = -1;		// select the 0.1 V digit.
	
	grid_DDS1.AddItem(&ico_DDS1, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS1.AddItem(&lbl_DDS1_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS1.AddItem(&ico_DDS1_Frequency, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS1.AddItem(&numCtrl_DDS1_Frequency, 1, 1, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_DDS1.AddItem(&ico_DDS1_SignalForm, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS1.AddItem(&enumCtrl_DDS1_SignalForm, 1, 2, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_DDS1.AddItem(&ico_DDS1_Amplitude, 0, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS1.AddItem(&numCtrl_DDS1_Amplitude, 1, 3, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_DDS1.AddItem(&ico_DDS1_Offset, 0, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS1.AddItem(&numCtrl_DDS1_Offset, 1, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS1.AddItem(&ico_DDS1_Enabled, 0, 5, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS1.AddItem(&boolCtrl_DDS1_Enabled, 1, 5, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS1.AddItem(&ico_DDS1_PWM, 2, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS1.AddItem(&numCtrl_DDS1_PWMValue, 3, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS1.AddItem(&button_DDS1_Config5VPWM, 2, 5, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_DDS1.InitItems();

	numCtrl_DDS2_Amplitude.CurrentDigitPosition = -1;	// select the 0.1 V digit.
	numCtrl_DDS2_Offset.CurrentDigitPosition = -1;		// select the 0.1 V digit.
	
	grid_DDS2.AddItem(&ico_DDS2, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS2.AddItem(&lbl_DDS2_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS2.AddItem(&ico_DDS2_Frequency, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS2.AddItem(&numCtrl_DDS2_Frequency, 1, 1, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_DDS2.AddItem(&ico_DDS2_SignalForm, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS2.AddItem(&enumCtrl_DDS2_SignalForm, 1, 2, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_DDS2.AddItem(&ico_DDS2_Amplitude, 0, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS2.AddItem(&numCtrl_DDS2_Amplitude, 1, 3, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_DDS2.AddItem(&ico_DDS2_Offset, 0, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS2.AddItem(&numCtrl_DDS2_Offset, 1, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS2.AddItem(&ico_DDS2_Enabled, 0, 5, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS2.AddItem(&boolCtrl_DDS2_Enabled, 1, 5, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS2.AddItem(&ico_DDS2_PWM, 2, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS2.AddItem(&numCtrl_DDS2_PWMValue, 3, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_DDS2.AddItem(&button_DDS2_Config5VPWM, 2, 5, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_DDS2.InitItems();
	
	return &list_DDS;
}

#endif /* DDS_SUBSYSTEM_ENABLED */