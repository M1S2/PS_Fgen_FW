/*
 * ScreenSettings.cpp
 * Created: 09.04.2021 19:17:59
 * Author: Markus Scheich
 */ 

#include "../Device.h"

ContainerList list_Settings(3, 20);

void SettingsChanged(void* context);

// ***** Settings Device page *****
void OnButtonSettingsSave(void* context);
void OnButtonDeviceCalibration(void* context);
void OnButtonDeviceReset(void* context);
void OnResetConfirmation(void* context);
void OnResetCancel(void* context);
void OnButtonResetCal(void* context);
void OnResetCalConfirmation(void* context);
void OnResetCalCancel(void* context);

ContainerGrid grid_Settings_Device(10, 5, 2, false, true);
Icon ico_settings_Device(icon_settings_width, icon_settings_height, icon_settings_bits, COLOR_FOREGROUND_HEADERS);
Label lbl_Settings_Device_caption("Settings Device", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 20);
Icon ico_Settings_Save(icon_save_width, icon_save_height, icon_save_bits);
ButtonControl button_Settings_Save("Save Settings", &Device, &OnButtonSettingsSave, 14);
Icon ico_Settings_Cal(icon_calibration_width, icon_calibration_height, icon_calibration_bits);
ButtonControl button_Settings_Calibration("Calibration...", &Device, &OnButtonDeviceCalibration, 15);
Icon ico_Settings_Reset(icon_reset_width, icon_reset_height, icon_reset_bits);
ButtonControl button_Settings_Reset("Reset Device", &Device, &OnButtonDeviceReset, 13);
MessageDialog msg_Settings_ResetConfirmation(MSG_DIALOG_MARGIN, MSG_DIALOG_MARGIN, DISPLAY_WIDTH - 2 * MSG_DIALOG_MARGIN, DISPLAY_HEIGHT - 2 * MSG_DIALOG_MARGIN, "Really reset the device?\nThis can't be undone!", MSG_WARNING, MSG_BTN_OK_CANCEL, &Device, &OnResetConfirmation, &OnResetCancel, 50);
Icon ico_Settings_ResetCal(icon_reset_width, icon_reset_height, icon_reset_bits);
ButtonControl button_Settings_ResetCal("Reset Cal.", &Device, &OnButtonResetCal, 11);
MessageDialog msg_Settings_ResetCalConfirmation(MSG_DIALOG_MARGIN, MSG_DIALOG_MARGIN, DISPLAY_WIDTH - 2 * MSG_DIALOG_MARGIN, DISPLAY_HEIGHT - 2 * MSG_DIALOG_MARGIN, "Really reset the cal.?\nA new calibration must be done!", MSG_WARNING, MSG_BTN_OK_CANCEL, &Device, &OnResetCalConfirmation, &OnResetCalCancel, 60);

// ***** Settings PowerUp page *****
ContainerGrid grid_Settings_PowerUp(5, 2, 4, false, true);
Icon ico_settings_PowerUp(icon_settings_width, icon_settings_height, icon_settings_bits, COLOR_FOREGROUND_HEADERS);
Label lbl_Settings_PowerUp_caption("Settings Power Up", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 20);
Icon ico_Settings_PowerUp_OutputStates(icon_boot_width, icon_boot_height, icon_boot_bits);
EnumControl<DevicePowerUpOutputEnabledStates_t> enumCtrl_Settings_PowerUp_OutputStates(&Device.PowerOnOutputsState, DevicePowerUpOutputEnabledStateNames, NUM_DEV_POWERUP_ELEMENTS, NULL, &SettingsChanged);
Label lbl_Settings_PowerUp_OutputStates("Output States", LABEL_COLOR_NOTSET, NULL, 0, 0, 15);

// ***** Settings Version Info page *****
ContainerGrid grid_Settings_VersionInfo(5, 4, 2, false, true);
Icon ico_versionInfo(icon_info_width, icon_info_height, icon_info_bits, COLOR_FOREGROUND_HEADERS);
Label lbl_Settings_VersionInfo_caption("Version Info", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 15);
Label lbl_Settings_VersionInfo_manufacturer("by " SCPI_IDN_MANUFACTURER, LABEL_COLOR_NOTSET, NULL, 0, 0, 20);
Label lbl_Settings_VersionInfo_serialNo("SNo.: " SCPI_IDN_SERIAL_NUMBER, LABEL_COLOR_NOTSET, NULL, 0, 0, 10);
Label lbl_Settings_VersionInfo_swVersion("SW: " SCPI_IDN_SOFTWARE_REVISION, LABEL_COLOR_NOTSET, NULL, 0, 0, 20);


void SettingsChanged(void* context)
{
	Device.SetSettingsChanged(true);
}

void OnButtonSettingsSave(void* context)
{
	Device.SaveSettings();
}

void OnButtonDeviceCalibration(void* context)
{
	Device.ScreenManager.ShowUiCalibrationMenu();
}

void OnButtonDeviceReset(void* context)
{
	UiManager.ChangeVisualTreeRoot(&msg_Settings_ResetConfirmation);
}

void OnResetConfirmation(void* context)
{
	Device.ResetDevice();
	Device.ScreenManager.ShowUiMainPage();
}

void OnResetCancel(void* context)
{
	Device.ScreenManager.ShowUiMainPage();
}

void OnButtonResetCal(void* context)
{
	UiManager.ChangeVisualTreeRoot(&msg_Settings_ResetCalConfirmation);
}

void OnResetCalConfirmation(void* context)
{
	Device.ResetDeviceCalibration();
	Device.ScreenManager.ShowUiMainPage();
}

void OnResetCalCancel(void* context)
{
	Device.ScreenManager.ShowUiMainPage();
}


UIElement* uiBuildScreenSettings()
{
	list_Settings.AddItem(&grid_Settings_Device);
	list_Settings.AddItem(&grid_Settings_PowerUp);
	list_Settings.AddItem(&grid_Settings_VersionInfo);

	grid_Settings_Device.AddItem(&ico_settings_Device, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.AddItem(&lbl_Settings_Device_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.AddItem(&ico_Settings_Save, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.AddItem(&button_Settings_Save, 1, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.AddItem(&ico_Settings_Cal, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.AddItem(&button_Settings_Calibration, 1, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.AddItem(&ico_Settings_Reset, 0, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.AddItem(&button_Settings_Reset, 1, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.AddItem(&ico_Settings_ResetCal, 0, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.AddItem(&button_Settings_ResetCal, 1, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.InitItems();

	grid_Settings_PowerUp.SetColumnWidth(2, 10);	// Use this column with a fixed width to add some space between the controls and the text
	grid_Settings_PowerUp.AddItem(&ico_settings_PowerUp, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_PowerUp.AddItem(&lbl_Settings_PowerUp_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_Settings_PowerUp.AddItem(&ico_Settings_PowerUp_OutputStates, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_PowerUp.AddItem(&enumCtrl_Settings_PowerUp_OutputStates, 1, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_PowerUp.AddItem(&lbl_Settings_PowerUp_OutputStates, 3, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_PowerUp.InitItems();
	
	grid_Settings_VersionInfo.AddItem(&ico_versionInfo, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_VersionInfo.AddItem(&lbl_Settings_VersionInfo_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_VersionInfo.AddItem(&lbl_Settings_VersionInfo_manufacturer, 0, 1, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_Settings_VersionInfo.AddItem(&lbl_Settings_VersionInfo_serialNo, 0, 2, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_Settings_VersionInfo.AddItem(&lbl_Settings_VersionInfo_swVersion, 0, 3, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_Settings_VersionInfo.InitItems();
		
	return &list_Settings;
}
