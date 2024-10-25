/*
 * ScreenSettings.cpp
 * Created: 09.04.2021 19:17:59
 * Author: Markus Scheich
 */ 

#include "../Device.h"

ContainerListDefault list_Settings;

void SettingsChanged(void* context);

// ***** Settings Device page *****
void OnButtonSettingsSave(void* context);
void OnButtonDeviceCalibration(void* context);
void OnButtonDeviceReset(void* context);
void OnResetConfirmation(void* context);
void OnResetCancel(void* context);

ContainerGridDefault grid_Settings_Device;
Icon ico_settings_Device(icon_settings_width, icon_settings_height, icon_settings_bits, COLOR_FOREGROUND_HEADERS);
Label<20> lbl_Settings_Device_caption("Settings Device", COLOR_FOREGROUND_HEADERS);
ButtonControl<14> button_Settings_Save("Save Settings", &Device, &OnButtonSettingsSave);
ButtonControl<15> button_Settings_Calibration("Calibration...", &Device, &OnButtonDeviceCalibration);
ButtonControl<13> button_Settings_Reset("Reset Device", &Device, &OnButtonDeviceReset);
MessageDialog<50> msg_Settings_ResetConfirmation(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, "Really reset the device?\nThis can't be undone!", MSG_WARNING, MSG_BTN_OK_CANCEL, &Device, &OnResetConfirmation, &OnResetCancel);

// ***** Settings Communication page *****
void SettingsCommunicationBaudRateChanged(void* context);

ContainerGrid<8, 3, 4, false, true> grid_Settings_Communication;
Icon ico_Settings_Communication(icon_serial_width, icon_serial_height, icon_serial_bits, COLOR_FOREGROUND_HEADERS);
Label<25> lbl_Settings_Communication_caption("Settings Communication", COLOR_FOREGROUND_HEADERS);

Icon ico_Settings_Comm_BaudRate(icon_speed_width, icon_speed_height, icon_speed_bits);
EnumControl<DeviceBaudRates_t> enumCtrl_Settings_Comm_BaudRate(&Device.SerialBaudRate, DeviceBaudRateNames, NUM_DEV_BAUD_ELEMENTS, &Device, &SettingsCommunicationBaudRateChanged);
Label<20> lbl_Settings_Comm_BaudRate("Serial Baud Rate");

Icon ico_Settings_Comm_Echo(icon_echo_width, icon_echo_height, icon_echo_bits);
BoolControl boolCtrl_Settings_Comm_Echo(&Device.SerialEchoEnabled, NULL, &SettingsChanged);
Label<15> lbl_Settings_Comm_Echo("Serial Echo");

// ***** Settings PowerUp page *****
ContainerGrid<5, 2, 4, false, true> grid_Settings_PowerUp;
Icon ico_settings_PowerUp(icon_settings_width, icon_settings_height, icon_settings_bits, COLOR_FOREGROUND_HEADERS);
Label<20> lbl_Settings_PowerUp_caption("Settings Power Up", COLOR_FOREGROUND_HEADERS);
Icon ico_Settings_PowerUp_OutputStates(icon_boot_width, icon_boot_height, icon_boot_bits);
EnumControl<DevicePowerUpOutputEnabledStates_t> enumCtrl_Settings_PowerUp_OutputStates(&Device.PowerOnOutputsState, DevicePowerUpOutputEnabledStateNames, NUM_DEV_POWERUP_ELEMENTS, NULL, &SettingsChanged);
Label<15> lbl_Settings_PowerUp_OutputStates("Output States");

// ***** Settings Version Info page *****
ContainerGrid<5, 4, 2, false, true> grid_Settings_VersionInfo;
Icon ico_versionInfo(icon_info_width, icon_info_height, icon_info_bits, COLOR_FOREGROUND_HEADERS);
Label<15> lbl_Settings_VersionInfo_caption("Version Info", COLOR_FOREGROUND_HEADERS);
Label<20> lbl_Settings_VersionInfo_manufacturer("by " SCPI_IDN_MANUFACTURER);
Label<10> lbl_Settings_VersionInfo_serialNo("SNo.: " SCPI_IDN_SERIAL_NUMBER);
Label<15> lbl_Settings_VersionInfo_swVersion("SW: " SCPI_IDN_SOFTWARE_REVISION);


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

void SettingsCommunicationBaudRateChanged(void* context)
{
	DeviceBaudRates_t baudRate = Device.SerialBaudRate;
	Device.SerialBaudRate = (baudRate == DEV_BAUD_110 ? DEV_BAUD_150 : DEV_BAUD_110);		//Set the SerialBaudRate to a value that is different from the set baud rate (to trigger the baud rate switch in SetSerialBaudRate())
	Device.SetSerialBaudRate(baudRate);
}

UIElement* uiBuildScreenSettings()
{
	list_Settings.AddItem(&grid_Settings_Device);
	list_Settings.AddItem(&grid_Settings_Communication);
	list_Settings.AddItem(&grid_Settings_PowerUp);
	list_Settings.AddItem(&grid_Settings_VersionInfo);

	grid_Settings_Device.AddItem(&ico_settings_Device, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.AddItem(&lbl_Settings_Device_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Device.AddItem(&button_Settings_Save, 0, 1, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_Settings_Device.AddItem(&button_Settings_Calibration, 0, 2, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_Settings_Device.AddItem(&button_Settings_Reset, 0, 3, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_Settings_Device.InitItems();

	grid_Settings_Communication.SetColumnWidth(2, 10);	// Use this column with a fixed width to add some space between the controls and the text
	grid_Settings_Communication.AddItem(&ico_Settings_Communication, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Communication.AddItem(&lbl_Settings_Communication_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_Settings_Communication.AddItem(&ico_Settings_Comm_BaudRate, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Communication.AddItem(&enumCtrl_Settings_Comm_BaudRate, 1, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Communication.AddItem(&lbl_Settings_Comm_BaudRate, 3, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Communication.AddItem(&ico_Settings_Comm_Echo, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Communication.AddItem(&boolCtrl_Settings_Comm_Echo, 1, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Communication.AddItem(&lbl_Settings_Comm_Echo, 3, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_Settings_Communication.InitItems();

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
