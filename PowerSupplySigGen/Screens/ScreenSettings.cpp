/*
 * ScreenSettings.cpp
 *
 * Created: 09.04.2021 19:17:59
 *  Author: V17
 */ 

#include "../Device.h"

#define SETTINGS_COLUMN1_POSX		SCREEN_TAB_WIDTH + 5
#define SETTINGS_COLUMN2_POSX		SETTINGS_COLUMN1_POSX + 90
#define SETTINGS_ROW1_POSY			25
#define SETTINGS_ROW2_POSY			SETTINGS_ROW1_POSY + 20

ContainerList list_Settings(SCREEN_TAB_WIDTH, 0, 240 - SCREEN_TAB_WIDTH, 64);

Icon ico_settings(SCREEN_TAB_WIDTH + 5, 3, icon_settings_width, icon_settings_height, icon_settings_bits);

void SettingsChanged(void* context);

// ***** Settings Device page *****
void OnButtonSettingsSave(void* context);
void OnButtonDeviceReset(void* context);
void OnResetConfirmation(void* context);
void OnResetCancel(void* context);

ContainerPage page_Settings_Device;
Label<20> lbl_Settings_Device_caption(SCREEN_TAB_WIDTH + 25, 5, "Settings Device");
ButtonControl button_Settings_Save(SETTINGS_COLUMN1_POSX, SETTINGS_ROW1_POSY, 70, DEFAULT_UI_ELEMENT_HEIGHT, "Save Settings", &Device, &OnButtonSettingsSave);
ButtonControl button_Settings_Reset(SETTINGS_COLUMN1_POSX, SETTINGS_ROW2_POSY, 70, DEFAULT_UI_ELEMENT_HEIGHT, "Reset Device", &Device, &OnButtonDeviceReset);
MessageDialog msg_Settings_ResetConfirmation(0, 0, 240, 64, "Do you really want to reset the device?\nThis can't be undone!", MSG_WARNING, MSG_BTN_OK_CANCEL, &Device, &OnResetConfirmation, &OnResetCancel);

// ***** Settings Display page *****
void SettingsDisplayInvertedChanged(void* context);

ContainerPage page_Settings_Display;
Label<20> lbl_Settings_Display_caption(SCREEN_TAB_WIDTH + 25, 5, "Settings Display");
Icon ico_Settings_Display_Inverse(SETTINGS_COLUMN1_POSX, SETTINGS_ROW1_POSY - 2, icon_colorInvert_width, icon_colorInvert_height, icon_colorInvert_bits);
BoolControl boolCtrl_Settings_Display_Inverse(SETTINGS_COLUMN1_POSX + icon_colorInvert_width + 3, SETTINGS_ROW1_POSY, &Device.ScreenManager.DisplayInverted, &Device.ScreenManager, &SettingsDisplayInvertedChanged);
Label<20> lbl_Settings_Display_Inverse(SETTINGS_COLUMN2_POSX, SETTINGS_ROW1_POSY, "Display Inverted");

// ***** Settings Communication page *****
void SettingsCommunicationBaudRateChanged(void* context);

ContainerPage page_Settings_Communication;
Icon ico_Settings_Communication(SCREEN_TAB_WIDTH + 5, 3, icon_serial_width, icon_serial_height, icon_serial_bits);
Label<25> lbl_Settings_Communication_caption(SCREEN_TAB_WIDTH + 25, 5, "Settings Communication");

Icon ico_Settings_Comm_BaudRate(SETTINGS_COLUMN1_POSX, SETTINGS_ROW1_POSY - 2, icon_speed_width, icon_speed_height, icon_speed_bits);
EnumControl<DeviceBaudRates_t> enumCtrl_Settings_Comm_BaudRate(SETTINGS_COLUMN1_POSX + icon_speed_width + 3, SETTINGS_ROW1_POSY, &Device.SerialBaudRate, DeviceBaudRateNames, 10, &Device, &SettingsCommunicationBaudRateChanged);
Label<20> lbl_Settings_Comm_BaudRate(SETTINGS_COLUMN2_POSX, SETTINGS_ROW1_POSY, "Serial Baud Rate");

Icon ico_Settings_Comm_Echo(SETTINGS_COLUMN1_POSX, SETTINGS_ROW2_POSY - 2, icon_echo_width, icon_echo_height, icon_echo_bits);
BoolControl boolCtrl_Settings_Comm_Echo(SETTINGS_COLUMN1_POSX + icon_echo_width + 3, SETTINGS_ROW2_POSY, &Device.SerialEchoEnabled, NULL, &SettingsChanged);
Label<15> lbl_Settings_Comm_Echo(SETTINGS_COLUMN2_POSX, SETTINGS_ROW2_POSY, "Serial Echo");

// ***** Settings PowerUp page *****
ContainerPage page_Settings_PowerUp;
Label<20> lbl_Settings_PowerUp_caption(SCREEN_TAB_WIDTH + 25, 5, "Settings Power Up");
Icon ico_Settings_PowerUp_OutputStates(SETTINGS_COLUMN1_POSX, SETTINGS_ROW1_POSY - 2, icon_boot_width, icon_boot_height, icon_boot_bits);
EnumControl<DevicePowerUpOutputEnabledStates_t> enumCtrl_Settings_PowerUp_OutputStates(SETTINGS_COLUMN1_POSX + icon_boot_width + 3, SETTINGS_ROW1_POSY, &Device.PowerOnOutputsState, DevicePowerUpOutputEnabledStateNames, 3, NULL, &SettingsChanged);
Label<15> lbl_Settings_PowerUp_OutputStates(SETTINGS_COLUMN2_POSX, SETTINGS_ROW1_POSY, "Output States");

// ***** Settings Version Info page *****
ContainerPage page_Settings_VersionInfo;
Icon ico_versionInfo(SCREEN_TAB_WIDTH + 5, 3, icon_info_width, icon_info_height, icon_info_bits);
Label<15> lbl_Settings_VersionInfo_caption(SCREEN_TAB_WIDTH + 25, 5, "Version Info");
LabelDefault lbl_Settings_VersionInfo_manufacturer(SETTINGS_COLUMN1_POSX, SETTINGS_ROW1_POSY - 2, "by " SCPI_IDN_MANUFACTURER);
LabelDefault lbl_Settings_VersionInfo_serialNo(SETTINGS_COLUMN1_POSX, SETTINGS_ROW1_POSY + 10, "SNo.: " SCPI_IDN_SERIAL_NUMBER);
LabelDefault lbl_Settings_VersionInfo_swVersion(SETTINGS_COLUMN1_POSX, SETTINGS_ROW1_POSY + 22, "SW: " SCPI_IDN_SOFTWARE_REVISION);


void SettingsChanged(void* context)
{
	Device.SetSettingsChanged(true);
}

void OnButtonSettingsSave(void* context)
{
	Device.SaveSettings();
}

void OnButtonDeviceReset(void* context)
{
	Device.ScreenManager.UiManager.ChangeVisualTreeRoot(&msg_Settings_ResetConfirmation);
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

void SettingsDisplayInvertedChanged(void* context)
{
	Device.ScreenManager.SetDisplayInverted(Device.ScreenManager.DisplayInverted);
	Device.SetSettingsChanged(true);
}

void SettingsCommunicationBaudRateChanged(void* context)
{
	DeviceBaudRates_t baudRate = Device.SerialBaudRate;
	Device.SerialBaudRate = (baudRate == DEV_BAUD_110 ? DEV_BAUD_150 : DEV_BAUD_110);		//Set the SerialBaudRate to a value that is different from the set baud rate (to trigger the baud rate switch in SetSerialBaudRate())
	Device.SetSerialBaudRate(baudRate);
}

UIElement* uiBuildScreenSettings()
{
	page_Settings_Device.AddItem(&ico_settings);
	page_Settings_Device.AddItem(&lbl_Settings_Device_caption);
	page_Settings_Device.AddItem(&button_Settings_Save);
	page_Settings_Device.AddItem(&button_Settings_Reset);
	page_Settings_Device.InitItems();
	
	page_Settings_Display.AddItem(&ico_settings);
	page_Settings_Display.AddItem(&lbl_Settings_Display_caption);
	page_Settings_Display.AddItem(&ico_Settings_Display_Inverse);
	page_Settings_Display.AddItem(&boolCtrl_Settings_Display_Inverse);
	page_Settings_Display.AddItem(&lbl_Settings_Display_Inverse);
	page_Settings_Display.InitItems();

	page_Settings_Communication.AddItem(&ico_Settings_Communication);
	page_Settings_Communication.AddItem(&lbl_Settings_Communication_caption);
	page_Settings_Communication.AddItem(&ico_Settings_Comm_BaudRate);
	page_Settings_Communication.AddItem(&enumCtrl_Settings_Comm_BaudRate);
	page_Settings_Communication.AddItem(&lbl_Settings_Comm_BaudRate);
	page_Settings_Communication.AddItem(&ico_Settings_Comm_Echo);
	page_Settings_Communication.AddItem(&boolCtrl_Settings_Comm_Echo);
	page_Settings_Communication.AddItem(&lbl_Settings_Comm_Echo);
	page_Settings_Communication.InitItems();

	page_Settings_PowerUp.AddItem(&ico_settings);
	page_Settings_PowerUp.AddItem(&lbl_Settings_PowerUp_caption);
	page_Settings_PowerUp.AddItem(&ico_Settings_PowerUp_OutputStates);
	page_Settings_PowerUp.AddItem(&enumCtrl_Settings_PowerUp_OutputStates);
	page_Settings_PowerUp.AddItem(&lbl_Settings_PowerUp_OutputStates);
	page_Settings_PowerUp.InitItems();
	
	page_Settings_VersionInfo.AddItem(&ico_versionInfo);
	page_Settings_VersionInfo.AddItem(&lbl_Settings_VersionInfo_caption);
	page_Settings_VersionInfo.AddItem(&lbl_Settings_VersionInfo_manufacturer);
	page_Settings_VersionInfo.AddItem(&lbl_Settings_VersionInfo_serialNo);
	page_Settings_VersionInfo.AddItem(&lbl_Settings_VersionInfo_swVersion);
	page_Settings_VersionInfo.InitItems();
	
	list_Settings.AddItem(&page_Settings_Device);
	list_Settings.AddItem(&page_Settings_Display);
	list_Settings.AddItem(&page_Settings_Communication);
	list_Settings.AddItem(&page_Settings_PowerUp);
	list_Settings.AddItem(&page_Settings_VersionInfo);
		
	return &list_Settings;
}
