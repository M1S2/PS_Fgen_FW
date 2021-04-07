/*
 * ScreenPS.cpp
 *
 * Created: 07.11.2020 13:09:35
 *  Author: V17
 */ 

#include "../Device.h"
#include "../UI_Lib/Indicators/NumericIndicator.cpp"
#include "../UI_Lib/Controls/NumericControl.cpp"
#include "../UI_Lib/Indicators/EnumIndicator.cpp"
#include "../UI_Lib/Controls/EnumControl.cpp"

ContainerList list_PS(SCREEN_TAB_WIDTH, 0, 240 - SCREEN_TAB_WIDTH, 64);

// ***** Power Supply Overview page *****
Icon ico_PSOverview(SCREEN_TAB_WIDTH + 5, 3, icon_supplyDC_width, icon_supplyDC_height, icon_supplyDC_bits);
Label lbl_PSOverview_caption(SCREEN_TAB_WIDTH + 25, 5, "PowerSupply");

Icon ico_PSOverviewVoltage(SCREEN_TAB_WIDTH + 5, 23, icon_voltage_width, icon_voltage_height, icon_voltage_bits);
NumericControl<float> numCtrl_PSOverviewVoltage(SCREEN_TAB_WIDTH + 23, 25, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, &Device.PsChannel.Amplitude.Val, "V", PS_MIN_AMPLITUDE, PS_MAX_AMPLITUDE, 3, &Device.PsChannel, &PS_Channel::PSAmplitudeChanged);
Icon ico_PSOverviewCurrent(SCREEN_TAB_WIDTH + 5, 43, icon_current_width, icon_current_height, icon_current_bits);
NumericControl<float> numCtrl_PSOverviewCurrent(SCREEN_TAB_WIDTH + 23, 45, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, &Device.PsChannel.Current.Val, "A", PS_MIN_CURRENT, PS_MAX_CURRENT, 3, &Device.PsChannel, &PS_Channel::PSCurrentChanged);
Icon ico_PSOverviewEnabled(SCREEN_TAB_WIDTH + 95, 23, icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_PSOverviewEnabled(SCREEN_TAB_WIDTH + 113, 25, 25, DEFAULT_UI_ELEMENT_HEIGHT, &Device.PsChannel.Enabled.Val, &Device.PsChannel, &PS_Channel::PSEnabledChanged);

NumericIndicator<float> numInd_PsOverviewVoltage(SCREEN_TAB_WIDTH + 151, 18, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, &Device.PsChannel.MeasuredAmplitude, "V", PS_MAX_AMPLITUDE, 3);
NumericIndicator<float> numInd_PsOverviewCurrent(SCREEN_TAB_WIDTH + 151, 28, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, &Device.PsChannel.MeasuredCurrent, "A", PS_MAX_CURRENT, 3);
NumericIndicator<float> numInd_PsOverviewPower(SCREEN_TAB_WIDTH + 151, 38, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, &Device.PsChannel.MeasuredPower, "W", PS_MAX_AMPLITUDE * PS_MAX_CURRENT, 3);
EnumIndicator<PsStates_t> enumInd_PsOverviewState(SCREEN_TAB_WIDTH + 155, 48, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, &Device.PsChannel.PsState, PSStatesNames, 5);

ContainerPage page_PSOverview;

// ***** Power Supply Protection OVP page *****
Icon ico_PSProtection(40, 3, icon_protection_width, icon_protection_height, icon_protection_bits);
Label lbl_PSProtectionOVP_caption(60, 5, "OVP");
ContainerPage page_PSProtectionOVP;

// ***** Power Supply Protection OCP page *****
Label lbl_PSProtectionOCP_caption(60, 5, "OCP");
ContainerPage page_PSProtectionOCP;

// ***** Power Supply Protection OPP page *****
Label lbl_PSProtectionOPP_caption(60, 5, "OPP");
ContainerPage page_PSProtectionOPP;

UIElement* uiBuildScreenPS()
{
	page_PSOverview.AddItem(&ico_PSOverview);
	page_PSOverview.AddItem(&lbl_PSOverview_caption);
	page_PSOverview.AddItem(&ico_PSOverviewVoltage);
	page_PSOverview.AddItem(&numCtrl_PSOverviewVoltage);
	page_PSOverview.AddItem(&ico_PSOverviewCurrent);
	page_PSOverview.AddItem(&numCtrl_PSOverviewCurrent);
	page_PSOverview.AddItem(&ico_PSOverviewEnabled);
	page_PSOverview.AddItem(&boolCtrl_PSOverviewEnabled);
	page_PSOverview.AddItem(&numInd_PsOverviewVoltage);
	page_PSOverview.AddItem(&numInd_PsOverviewCurrent);
	page_PSOverview.AddItem(&numInd_PsOverviewPower);
	page_PSOverview.AddItem(&enumInd_PsOverviewState);
	page_PSOverview.InitItems();

	page_PSProtectionOVP.AddItem(&ico_PSProtection);
	page_PSProtectionOVP.AddItem(&lbl_PSProtectionOVP_caption);
	page_PSProtectionOVP.InitItems();

	page_PSProtectionOCP.AddItem(&ico_PSProtection);
	page_PSProtectionOCP.AddItem(&lbl_PSProtectionOCP_caption);
	page_PSProtectionOCP.InitItems();
	
	page_PSProtectionOPP.AddItem(&ico_PSProtection);
	page_PSProtectionOPP.AddItem(&lbl_PSProtectionOPP_caption);
	page_PSProtectionOPP.InitItems();
			
	list_PS.AddItem(&page_PSOverview);
	list_PS.AddItem(&page_PSProtectionOVP);
	list_PS.AddItem(&page_PSProtectionOCP);
	list_PS.AddItem(&page_PSProtectionOPP);
						
	return &list_PS;
}