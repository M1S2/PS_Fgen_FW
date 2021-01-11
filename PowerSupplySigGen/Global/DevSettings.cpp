/*
 * DevSettings.cpp
 *
 * Created: 13.11.2020 19:31:33
 *  Author: V17
 */ 

#include "DevSettings.h"
#include "../Outputs/PowerSupply.h"
#include "../Screens/ScreenManager.h"
#include "../Device.h"
#include "../USART/USART.h"

/* This structure is only used internally to store to / read from EEPROM */
struct DevSettingsEEPROMLayout_t
{
	float PS_Voltage;
	//bool PS_Output_Enabled;
	float PS_LoadImpedance;
	
	uint8_t Screens_TabIndex;
	uint8_t Screens_Inverted;
	
	uint32_t Device_SerialBaudRate;
	bool Device_SerialEchoEnabled;
	
	float DDS1_Frequency;
	SignalForms_t DDS1_SignalForm;
	float DDS1_Amplitude;
	float DDS1_Offset;
};

bool DevSettingsNeedSaving;
DevSettingsEEPROMLayout_t EEMEM NonVolatileSettings;

void SaveSettings()
{
	DevSettingsEEPROMLayout_t settings;
	/* Collect setting from appropriate classes */
	settings.PS_Voltage = PowerSupply.Voltage;
	settings.PS_LoadImpedance = PowerSupply.LoadImpedance;
	settings.Screens_TabIndex = ScreenManager.TabIndex;
	settings.Screens_Inverted = ScreenManager.DisplayInverted;
	settings.Device_SerialBaudRate = Device.SerialBaudRate;
	settings.Device_SerialEchoEnabled = Device.SerialEchoEnabled;
	settings.DDS1_Frequency = DDS_Channel1.GetFrequency();
	settings.DDS1_SignalForm = DDS_Channel1.GetSignalForm();
	settings.DDS1_Amplitude = DDS_Channel1.GetAmplitude();
	settings.DDS1_Offset = DDS_Channel1.GetOffset();

	eeprom_write_block((const void*)&settings, (void*)&NonVolatileSettings, sizeof(DevSettingsEEPROMLayout_t));
	
	DevSettingsNeedSaving = false;
}

void LoadSettings()
{
	DevSettingsEEPROMLayout_t settings;
	eeprom_read_block((void*)&settings, (const void*)&NonVolatileSettings, sizeof(DevSettingsEEPROMLayout_t));
	
	/* Assign Settings to appropriate classes */
	PowerSupply.Voltage = settings.PS_Voltage;
	PowerSupply.OutputEnabled = false;
	PowerSupply.LoadImpedance = settings.PS_LoadImpedance;
	PowerSupply.UpdateOutput();
	
	ScreenManager.TabIndex = settings.Screens_TabIndex;
	ScreenManager.SetDisplayInverted(settings.Screens_Inverted);
	
	Device.SetSerialBaudRate(settings.Device_SerialBaudRate);
	Device.SetSerialEchoEnabled(settings.Device_SerialEchoEnabled);
	
	DDS_Channel1.SetFrequency(settings.DDS1_Frequency);
	DDS_Channel1.SetSignalForm(settings.DDS1_SignalForm);
	DDS_Channel1.SetAmplitude(settings.DDS1_Amplitude);
	DDS_Channel1.SetOffset(settings.DDS1_Offset);
	DDS_Channel1.UpdateWaveTable();
	
	DevSettingsNeedSaving = false;
}

void ResetDevice()
{	
	PowerSupply.Voltage = 5;
	PowerSupply.OutputEnabled = false;
	PowerSupply.LoadImpedance = 1000000;
	PowerSupply.UpdateOutput();
	ScreenManager.TabIndex = 0;
	ScreenManager.SetDisplayEnabled(true);
	ScreenManager.SetDisplayInverted(false);
	DDS_Channel1.SetFrequency(1000);
	DDS_Channel1.SetSignalForm(SINE);
	DDS_Channel1.SetAmplitude(10);
	DDS_Channel1.SetOffset(0);
	SaveSettings();
}
