/*
 * ScreenPS.cpp
 * Created: 07.11.2020 13:09:35
 * Author: Markus Scheich
 */ 

#include "../Device.h"

#ifdef PS_SUBSYSTEM_ENABLED

ContainerList list_PS(SCREEN_TAB_WIDTH, 0, 240 - SCREEN_TAB_WIDTH, 64);

#define PS_COLUMN1_POSX		SCREEN_TAB_WIDTH + 5
#define PS_COLUMN2_POSX		PS_COLUMN1_POSX + 90
#define PS_COLUMN3_POSX		PS_COLUMN2_POSX + 56
#define PS_ROW1_POSY		25
#define PS_ROW2_POSY		PS_ROW1_POSY + 20

void PSProtectionsClear(void* controlContext);
void PSProtectionsClearedOK(void* controlContext);

// ***** Power Supply Overview page *****
ContainerPage page_PSOverview;
Icon ico_PSOverview(SCREEN_TAB_WIDTH + 5, 3, icon_supplyDC_width, icon_supplyDC_height, icon_supplyDC_bits);
Label<15> lbl_PSOverview_caption(SCREEN_TAB_WIDTH + 25, 5, "PowerSupply");

Icon ico_PSOverviewVoltage(PS_COLUMN1_POSX, PS_ROW1_POSY - 2, icon_voltage_width, icon_voltage_height, icon_voltage_bits);
NumericControl<float> numCtrl_PSOverviewVoltage(PS_COLUMN1_POSX + icon_voltage_width + 3, PS_ROW1_POSY, &Device.PsChannel.Voltage.Val, "V", PS_MIN_VOLTAGE, PS_MAX_VOLTAGE, 3, &Device.PsChannel, &PS_Channel::PSVoltageChanged);
Icon ico_PSOverviewEnabled(PS_COLUMN2_POSX, PS_ROW1_POSY - 2, icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_PSOverviewEnabled(PS_COLUMN2_POSX + icon_OnOff_width + 3, PS_ROW1_POSY, &Device.PsChannel.Enabled.Val, &Device.PsChannel, &PS_Channel::PSEnabledChanged);
Icon ico_PSOverviewCurrent(PS_COLUMN1_POSX, PS_ROW2_POSY - 2, icon_current_width, icon_current_height, icon_current_bits);
NumericControl<float> numCtrl_PSOverviewCurrent(PS_COLUMN1_POSX + icon_current_width + 3, PS_ROW2_POSY, &Device.PsChannel.Current.Val, "A", PS_MIN_CURRENT, PS_MAX_CURRENT, 3, &Device.PsChannel, &PS_Channel::PSCurrentChanged);

NumericIndicator<float> numInd_PsOverviewVoltage(PS_COLUMN3_POSX, 18, &Device.PsChannel.MeasuredVoltage, "V", PS_MAX_VOLTAGE, 3);
NumericIndicator<float> numInd_PsOverviewCurrent(PS_COLUMN3_POSX, 28, &Device.PsChannel.MeasuredCurrent, "A", PS_MAX_CURRENT, 3);
NumericIndicator<float> numInd_PsOverviewPower(PS_COLUMN3_POSX, 38, &Device.PsChannel.MeasuredPower, "W", PS_MAX_VOLTAGE * PS_MAX_CURRENT, 3);
EnumIndicator<PsStates_t> enumInd_PsOverviewState(PS_COLUMN3_POSX + 4, 48, &Device.PsChannel.PsState, PSStatesNames, NUM_PS_STATE_ELEMENTS);

// ***** Power Supply Protection OVP page *****
ContainerPage page_PSProtectionOVP;
Icon ico_PSProtection(40, 3, icon_protection_width, icon_protection_height, icon_protection_bits);
Label<5> lbl_PSProtectionOVP_caption(60, 5, "OVP");
Icon ico_PSProtectionOVPLevel(PS_COLUMN1_POSX, PS_ROW1_POSY - 2, icon_level_width, icon_level_height, icon_level_bits);
NumericControl<uint8_t> numCtrl_PSProtectionOVPLevel(PS_COLUMN1_POSX + icon_level_width + 3, PS_ROW1_POSY, &Device.PsChannel.OvpLevel.Val, "%%", PS_MIN_OVP_LEVEL_PERCENTAGE, PS_MAX_OVP_LEVEL_PERCENTAGE, 0, &Device.PsChannel, &PS_Channel::PSOvpLevelChanged);
Icon ico_PSProtectionOVPState(PS_COLUMN2_POSX, PS_ROW1_POSY - 2, icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_PSProtectionOVPState(PS_COLUMN2_POSX + icon_OnOff_width + 3, PS_ROW1_POSY, &Device.PsChannel.OvpState.Val, &Device.PsChannel, &PS_Channel::PSOvpStateChanged);
Icon ico_PSProtectionOVPDelay(PS_COLUMN1_POSX, PS_ROW2_POSY - 2, icon_delay_width, icon_delay_height, icon_delay_bits);
NumericControl<float> numCtrl_PSProtectionOVPDelay(PS_COLUMN1_POSX + icon_delay_width + 3, PS_ROW2_POSY, &Device.PsChannel.OvpDelay.Val, "s", PS_MIN_OVP_DELAY, PS_MAX_OVP_DELAY, 3, &Device.PsChannel, &PS_Channel::PSOvpDelayChanged);
ButtonControl<6> button_PSProtectionOVPClear(PS_COLUMN2_POSX, PS_ROW2_POSY, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, "Clear", &Device.PsChannel, &PSProtectionsClear);

// ***** Power Supply Protection OCP page *****
ContainerPage page_PSProtectionOCP;
Label<5> lbl_PSProtectionOCP_caption(60, 5, "OCP");
Icon ico_PSProtectionOCPLevel(PS_COLUMN1_POSX, PS_ROW1_POSY - 2, icon_level_width, icon_level_height, icon_level_bits);
NumericControl<uint8_t> numCtrl_PSProtectionOCPLevel(PS_COLUMN1_POSX + icon_level_width + 3, PS_ROW1_POSY, &Device.PsChannel.OcpLevel.Val, "%%", PS_MIN_OCP_LEVEL_PERCENTAGE, PS_MAX_OCP_LEVEL_PERCENTAGE, 0, &Device.PsChannel, &PS_Channel::PSOcpLevelChanged);
Icon ico_PSProtectionOCPState(PS_COLUMN2_POSX, PS_ROW1_POSY - 2, icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_PSProtectionOCPState(PS_COLUMN2_POSX + icon_OnOff_width + 3, PS_ROW1_POSY, &Device.PsChannel.OcpState.Val, &Device.PsChannel, &PS_Channel::PSOcpStateChanged);
Icon ico_PSProtectionOCPDelay(PS_COLUMN1_POSX, PS_ROW2_POSY - 2, icon_delay_width, icon_delay_height, icon_delay_bits);
NumericControl<float> numCtrl_PSProtectionOCPDelay(PS_COLUMN1_POSX + icon_delay_width + 3, PS_ROW2_POSY, &Device.PsChannel.OcpDelay.Val, "s", PS_MIN_OCP_DELAY, PS_MAX_OCP_DELAY, 3, &Device.PsChannel, &PS_Channel::PSOcpDelayChanged);
ButtonControl<6> button_PSProtectionOCPClear(PS_COLUMN2_POSX, PS_ROW2_POSY, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, "Clear", &Device.PsChannel, &PSProtectionsClear);

// ***** Power Supply Protection OPP page *****
ContainerPage page_PSProtectionOPP;
Label<5> lbl_PSProtectionOPP_caption(60, 5, "OPP");
Icon ico_PSProtectionOPPLevel(PS_COLUMN1_POSX, PS_ROW1_POSY - 2, icon_level_width, icon_level_height, icon_level_bits);
NumericControl<float> numCtrl_PSProtectionOPPLevel(PS_COLUMN1_POSX + icon_level_width + 3, PS_ROW1_POSY, &Device.PsChannel.OppLevel.Val, "W", PS_MIN_OPP_LEVEL, PS_MAX_OPP_LEVEL, 3, &Device.PsChannel, &PS_Channel::PSOppLevelChanged);
Icon ico_PSProtectionOPPState(PS_COLUMN2_POSX, PS_ROW1_POSY - 2, icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_PSProtectionOPPState(PS_COLUMN2_POSX + icon_OnOff_width + 3, PS_ROW1_POSY, &Device.PsChannel.OppState.Val, &Device.PsChannel, &PS_Channel::PSOppStateChanged);
Icon ico_PSProtectionOPPDelay(PS_COLUMN1_POSX, PS_ROW2_POSY - 2, icon_delay_width, icon_delay_height, icon_delay_bits);
NumericControl<float> numCtrl_PSProtectionOPPDelay(PS_COLUMN1_POSX + icon_delay_width + 3, PS_ROW2_POSY, &Device.PsChannel.OppDelay.Val, "s", PS_MIN_OPP_DELAY, PS_MAX_OPP_DELAY, 3, &Device.PsChannel, &PS_Channel::PSOppDelayChanged);
ButtonControl<6> button_PSProtectionOPPClear(PS_COLUMN2_POSX, PS_ROW2_POSY, DEFAULT_UI_ELEMENT_WIDTH, DEFAULT_UI_ELEMENT_HEIGHT, "Clear", &Device.PsChannel, &PSProtectionsClear);


MessageDialog msg_protectionsCleared(0, 0, 240, 64, "Protections Cleared.", MSG_INFO, MSG_BTN_OK, NULL, &PSProtectionsClearedOK);


void PSProtectionsClear(void* controlContext)
{
	Device.PsChannel.ClearProtections();
	Device.ScreenManager.UiManager.ChangeVisualTreeRoot(&msg_protectionsCleared);
}

void PSProtectionsClearedOK(void* controlContext)
{
	Device.ScreenManager.ShowUiMainPage();
}

UIElement* uiBuildScreenPS()
{
	page_PSOverview.AddItem(&ico_PSOverview);
	page_PSOverview.AddItem(&lbl_PSOverview_caption);
	page_PSOverview.AddItem(&ico_PSOverviewVoltage);
	page_PSOverview.AddItem(&numCtrl_PSOverviewVoltage);
	page_PSOverview.AddItem(&ico_PSOverviewEnabled);
	page_PSOverview.AddItem(&boolCtrl_PSOverviewEnabled);
	page_PSOverview.AddItem(&ico_PSOverviewCurrent);
	page_PSOverview.AddItem(&numCtrl_PSOverviewCurrent);
	page_PSOverview.AddItem(&numInd_PsOverviewVoltage);
	page_PSOverview.AddItem(&numInd_PsOverviewCurrent);
	page_PSOverview.AddItem(&numInd_PsOverviewPower);
	page_PSOverview.AddItem(&enumInd_PsOverviewState);
	page_PSOverview.InitItems();

	page_PSProtectionOVP.AddItem(&ico_PSProtection);
	page_PSProtectionOVP.AddItem(&lbl_PSProtectionOVP_caption);
	page_PSProtectionOVP.AddItem(&ico_PSProtectionOVPLevel);
	page_PSProtectionOVP.AddItem(&numCtrl_PSProtectionOVPLevel);
	page_PSProtectionOVP.AddItem(&ico_PSProtectionOVPState);
	page_PSProtectionOVP.AddItem(&boolCtrl_PSProtectionOVPState);
	page_PSProtectionOVP.AddItem(&ico_PSProtectionOVPDelay);
	page_PSProtectionOVP.AddItem(&numCtrl_PSProtectionOVPDelay);
	page_PSProtectionOVP.AddItem(&button_PSProtectionOVPClear);
	page_PSProtectionOVP.InitItems();

	page_PSProtectionOCP.AddItem(&ico_PSProtection);
	page_PSProtectionOCP.AddItem(&lbl_PSProtectionOCP_caption);
	page_PSProtectionOCP.AddItem(&ico_PSProtectionOCPLevel);
	page_PSProtectionOCP.AddItem(&numCtrl_PSProtectionOCPLevel);
	page_PSProtectionOCP.AddItem(&ico_PSProtectionOCPState);
	page_PSProtectionOCP.AddItem(&boolCtrl_PSProtectionOCPState);
	page_PSProtectionOCP.AddItem(&ico_PSProtectionOCPDelay);
	page_PSProtectionOCP.AddItem(&numCtrl_PSProtectionOCPDelay);
	page_PSProtectionOCP.AddItem(&button_PSProtectionOCPClear);
	page_PSProtectionOCP.InitItems();
	
	page_PSProtectionOPP.AddItem(&ico_PSProtection);
	page_PSProtectionOPP.AddItem(&lbl_PSProtectionOPP_caption);
	page_PSProtectionOPP.AddItem(&ico_PSProtectionOPPLevel);
	page_PSProtectionOPP.AddItem(&numCtrl_PSProtectionOPPLevel);
	page_PSProtectionOPP.AddItem(&ico_PSProtectionOPPState);
	page_PSProtectionOPP.AddItem(&boolCtrl_PSProtectionOPPState);
	page_PSProtectionOPP.AddItem(&ico_PSProtectionOPPDelay);
	page_PSProtectionOPP.AddItem(&numCtrl_PSProtectionOPPDelay);
	page_PSProtectionOPP.AddItem(&button_PSProtectionOPPClear);
	page_PSProtectionOPP.InitItems();
			
	list_PS.AddItem(&page_PSOverview);
	list_PS.AddItem(&page_PSProtectionOVP);
	list_PS.AddItem(&page_PSProtectionOCP);
	list_PS.AddItem(&page_PSProtectionOPP);
						
	return &list_PS;
}

#endif /* PS_SUBSYSTEM_ENABLED */