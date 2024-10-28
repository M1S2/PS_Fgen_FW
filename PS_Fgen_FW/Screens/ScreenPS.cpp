/*
 * ScreenPS.cpp
 * Created: 07.11.2020 13:09:35
 * Author: Markus Scheich
 */ 

#include "../Device.h"

#ifdef PS_SUBSYSTEM_ENABLED

ContainerListDefault list_PS;

void PSProtectionsClear(void* controlContext);
void PSProtectionsClearedOK(void* controlContext);

// ***** Power Supply Overview page *****
ContainerGrid<13, 6, 3, false, true> grid_PSOverview;
Icon ico_PSOverview(icon_supplyDC_width, icon_supplyDC_height, icon_supplyDC_bits, COLOR_FOREGROUND_HEADERS);
Label<15> lbl_PSOverview_caption("PowerSupply", COLOR_FOREGROUND_HEADERS);

Icon ico_PSOverviewVoltage(icon_voltage_width, icon_voltage_height, icon_voltage_bits);
NumericControl<float> numCtrl_PSOverviewVoltage(&Device.PsChannel.Voltage.Val, "V", PS_MIN_VOLTAGE, PS_MAX_VOLTAGE, 3, &Device.PsChannel, &PS_Channel::PSVoltageChanged);
Icon ico_PSOverviewEnabled(icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_PSOverviewEnabled(&Device.PsChannel.Enabled.Val, &Device.PsChannel, &PS_Channel::PSEnabledChanged);
Icon ico_PSOverviewRegMode(icon_pin_width, icon_pin_height, icon_pin_bits);
EnumControl<volatile PsRegulationModes_t> enumCtrl_PSOverviewRegMode(&Device.PsChannel.RegulationMode, PsRegulationModesNames, NUM_PS_REG_MODE_ELEMENTS, &Device.PsChannel, &PS_Channel::PSRegulationModeChanged);

Icon ico_PSOverviewDmm(icon_dmm_width, icon_dmm_height, icon_dmm_bits);
NumericIndicator<volatile float, 10> numInd_PsOverviewVoltage(&Device.PsChannel.MeasuredVoltage, "V", PS_MAX_VOLTAGE, 3);
NumericIndicator<volatile float, 10> numInd_PsOverviewCurrent(&Device.PsChannel.MeasuredCurrent, "A", PS_MAX_CURRENT, 3);
NumericIndicator<volatile float, 10> numInd_PsOverviewPower(&Device.PsChannel.MeasuredPower, "W", PS_MAX_VOLTAGE * PS_MAX_CURRENT, 3);
EnumIndicator<volatile PsStates_t> enumInd_PsOverviewState(&Device.PsChannel.PsState, PsStatesNames, NUM_PS_STATE_ELEMENTS);

// ***** Power Supply Protection OVP page *****
ContainerGrid<9, 3, 4, false, true> grid_PSProtectionOVP;
Icon ico_PSProtectionOVP(icon_protection_width, icon_protection_height, icon_protection_bits, COLOR_FOREGROUND_HEADERS);
Label<5> lbl_PSProtectionOVP_caption("OVP", COLOR_FOREGROUND_HEADERS);
Icon ico_PSProtectionOVPLevel(icon_level_width, icon_level_height, icon_level_bits);
NumericControl<uint8_t> numCtrl_PSProtectionOVPLevel(&Device.PsChannel.OvpLevel.Val, "%%", PS_MIN_OVP_LEVEL_PERCENTAGE, PS_MAX_OVP_LEVEL_PERCENTAGE, 0, &Device.PsChannel, &PS_Channel::PSOvpLevelChanged);
Icon ico_PSProtectionOVPState(icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_PSProtectionOVPState(&Device.PsChannel.OvpState.Val, &Device.PsChannel, &PS_Channel::PSOvpStateChanged);
Icon ico_PSProtectionOVPDelay(icon_delay_width, icon_delay_height, icon_delay_bits);
NumericControl<float> numCtrl_PSProtectionOVPDelay(&Device.PsChannel.OvpDelay.Val, "s", PS_MIN_OVP_DELAY, PS_MAX_OVP_DELAY, 3, &Device.PsChannel, &PS_Channel::PSOvpDelayChanged);
ButtonControl<6> button_PSProtectionOVPClear("Clear", &Device.PsChannel, &PSProtectionsClear);

// ***** Power Supply Protection OCP page *****
ContainerGrid<9, 3, 4, false, true> grid_PSProtectionOCP;
Icon ico_PSProtectionOCP(icon_protection_width, icon_protection_height, icon_protection_bits, COLOR_FOREGROUND_HEADERS);
Label<5> lbl_PSProtectionOCP_caption("OCP", COLOR_FOREGROUND_HEADERS);
Icon ico_PSProtectionOCPLevel(icon_level_width, icon_level_height, icon_level_bits);
NumericControl<float> numCtrl_PSProtectionOCPLevel(&Device.PsChannel.OcpLevel.Val, "A", PS_MIN_OCP_LEVEL, PS_MAX_OCP_LEVEL, 3, &Device.PsChannel, &PS_Channel::PSOcpLevelChanged);
Icon ico_PSProtectionOCPState(icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_PSProtectionOCPState(&Device.PsChannel.OcpState.Val, &Device.PsChannel, &PS_Channel::PSOcpStateChanged);
Icon ico_PSProtectionOCPDelay(icon_delay_width, icon_delay_height, icon_delay_bits);
NumericControl<float> numCtrl_PSProtectionOCPDelay(&Device.PsChannel.OcpDelay.Val, "s", PS_MIN_OCP_DELAY, PS_MAX_OCP_DELAY, 3, &Device.PsChannel, &PS_Channel::PSOcpDelayChanged);
ButtonControl<6> button_PSProtectionOCPClear("Clear", &Device.PsChannel, &PSProtectionsClear);

// ***** Power Supply Protection OPP page *****
ContainerGrid<9, 3, 4, false, true> grid_PSProtectionOPP;
Icon ico_PSProtectionOPP(icon_protection_width, icon_protection_height, icon_protection_bits, COLOR_FOREGROUND_HEADERS);
Label<5> lbl_PSProtectionOPP_caption("OPP", COLOR_FOREGROUND_HEADERS);
Icon ico_PSProtectionOPPLevel(icon_level_width, icon_level_height, icon_level_bits);
NumericControl<float> numCtrl_PSProtectionOPPLevel(&Device.PsChannel.OppLevel.Val, "W", PS_MIN_OPP_LEVEL, PS_MAX_OPP_LEVEL, 3, &Device.PsChannel, &PS_Channel::PSOppLevelChanged);
Icon ico_PSProtectionOPPState(icon_OnOff_width, icon_OnOff_height, icon_OnOff_bits);
BoolControl boolCtrl_PSProtectionOPPState(&Device.PsChannel.OppState.Val, &Device.PsChannel, &PS_Channel::PSOppStateChanged);
Icon ico_PSProtectionOPPDelay(icon_delay_width, icon_delay_height, icon_delay_bits);
NumericControl<float> numCtrl_PSProtectionOPPDelay(&Device.PsChannel.OppDelay.Val, "s", PS_MIN_OPP_DELAY, PS_MAX_OPP_DELAY, 3, &Device.PsChannel, &PS_Channel::PSOppDelayChanged);
ButtonControl<6> button_PSProtectionOPPClear("Clear", &Device.PsChannel, &PSProtectionsClear);


MessageDialog<25> msg_protectionsCleared(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, "Protections Cleared.", MSG_INFO, MSG_BTN_OK, NULL, &PSProtectionsClearedOK);

void PSProtectionsClear(void* controlContext)
{
	Device.PsChannel.ClearProtections();
	UiManager.ChangeVisualTreeRoot(&msg_protectionsCleared);
}

void PSProtectionsClearedOK(void* controlContext)
{
	Device.ScreenManager.ShowUiMainPage();
}

UIElement* uiBuildScreenPS()
{
	list_PS.AddItem(&grid_PSOverview);
	list_PS.AddItem(&grid_PSProtectionOVP);
	list_PS.AddItem(&grid_PSProtectionOCP);
	list_PS.AddItem(&grid_PSProtectionOPP);
	
	enumCtrl_PSOverviewRegMode.Width = 39;
	numCtrl_PSOverviewVoltage.CurrentDigitPosition = -1;	// select the 0.1 V digit.
	
	grid_PSOverview.AddItem(&ico_PSOverview, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSOverview.AddItem(&lbl_PSOverview_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSOverview.AddItem(&ico_PSOverviewVoltage, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSOverview.AddItem(&numCtrl_PSOverviewVoltage, 1, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSOverview.AddItem(&ico_PSOverviewRegMode, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSOverview.AddItem(&enumCtrl_PSOverviewRegMode, 1, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSOverview.AddItem(&ico_PSOverviewEnabled, 0, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSOverview.AddItem(&boolCtrl_PSOverviewEnabled, 1, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSOverview.AddItem(&ico_PSOverviewDmm, 2, 1, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_PSOverview.AddItem(&numInd_PsOverviewVoltage, 2, 2, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_PSOverview.AddItem(&numInd_PsOverviewCurrent, 2, 3, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_PSOverview.AddItem(&numInd_PsOverviewPower, 2, 4, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_PSOverview.AddItem(&enumInd_PsOverviewState, 2, 5, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_PSOverview.InitItems();

	numCtrl_PSProtectionOVPDelay.CurrentDigitPosition = -1;	// select the 0.1 s digit.

	grid_PSProtectionOVP.AddItem(&ico_PSProtectionOVP, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOVP.AddItem(&lbl_PSProtectionOVP_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOVP.AddItem(&ico_PSProtectionOVPLevel, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOVP.AddItem(&numCtrl_PSProtectionOVPLevel, 1, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOVP.AddItem(&ico_PSProtectionOVPState, 2, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOVP.AddItem(&boolCtrl_PSProtectionOVPState, 3, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOVP.AddItem(&ico_PSProtectionOVPDelay, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOVP.AddItem(&numCtrl_PSProtectionOVPDelay, 1, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOVP.AddItem(&button_PSProtectionOVPClear, 3, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOVP.InitItems();

	numCtrl_PSProtectionOCPDelay.CurrentDigitPosition = -1;	// select the 0.1 s digit.

	grid_PSProtectionOCP.AddItem(&ico_PSProtectionOCP, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOCP.AddItem(&lbl_PSProtectionOCP_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOCP.AddItem(&ico_PSProtectionOCPLevel, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOCP.AddItem(&numCtrl_PSProtectionOCPLevel, 1, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOCP.AddItem(&ico_PSProtectionOCPState, 2, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOCP.AddItem(&boolCtrl_PSProtectionOCPState, 3, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOCP.AddItem(&ico_PSProtectionOCPDelay, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOCP.AddItem(&numCtrl_PSProtectionOCPDelay, 1, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOCP.AddItem(&button_PSProtectionOCPClear, 3, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOCP.InitItems();

	numCtrl_PSProtectionOPPLevel.CurrentDigitPosition = -1;	// select the 0.1 s digit.
	numCtrl_PSProtectionOPPDelay.CurrentDigitPosition = -1;	// select the 0.1 s digit.
	
	grid_PSProtectionOPP.AddItem(&ico_PSProtectionOPP, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOPP.AddItem(&lbl_PSProtectionOPP_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOPP.AddItem(&ico_PSProtectionOPPLevel, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOPP.AddItem(&numCtrl_PSProtectionOPPLevel, 1, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOPP.AddItem(&ico_PSProtectionOPPState, 2, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOPP.AddItem(&boolCtrl_PSProtectionOPPState, 3, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOPP.AddItem(&ico_PSProtectionOPPDelay, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOPP.AddItem(&numCtrl_PSProtectionOPPDelay, 1, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOPP.AddItem(&button_PSProtectionOPPClear, 3, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_PSProtectionOPP.InitItems();
						
	return &list_PS;
}

#endif /* PS_SUBSYSTEM_ENABLED */