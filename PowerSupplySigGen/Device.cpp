/*
 * Device.cpp
 *
 * Created: 06.12.2020 14:44:51
 *  Author: V17
 */ 

#include "Device.h"
#include "Screens/ScreenManager.h"
#include "USART/USART.h"

#include <stdio.h>

DeviceClass Device;
DevSettingsEEPROMLayout_t EEMEM NonVolatileSettings;

bool DeviceClass::IsUserInputLocked() 
{
	 return (DeviceControlState != DEV_CTRL_LOCAL || strcmp(ScreenManager.UserMessage, "") != 0 || strcmp(ScreenManager.SystemMessage, "") != 0); 
}

void DeviceClass::UpdateControlStateOnUserInput()
{
	if(strcmp(ScreenManager.UserMessage, "") != 0)
	{
		strcpy(ScreenManager.UserMessage, "");
	}
	else
	{
		switch(DeviceControlState)
		{
			case DEV_CTRL_LOCAL: strcpy(ScreenManager.SystemMessage, ""); /* Nothing to do here. Device is already in local state. */ break;
			case DEV_CTRL_REMOTE: DeviceControlState = DEV_CTRL_LOCAL; break;
			case DEV_CTRL_RWLOCK: /* Nothing to do here. It is only possible to return from state RWLock via SCPI command. */ break;
		}
	}	
}

void DeviceClass::SetSerialBaudRate(uint32_t baud)
{
	if(baud != 110 && baud != 150 && baud != 300 && baud != 1200 && baud != 2400 && baud != 4800 && baud != 9600 && baud != 19200 && baud != 38400 && baud != 57600)		/* Check for invalid baud rates */
	{
		Usart0TransmitStr("Invalid baud rate selected. Nothing done.\r\n");
		return;
	}
	
	if(SerialBaudRate != baud && baud > 0 && baud < 115200)
	{
		DevSettingsNeedSaving = true;
		char buffer[60];
		sprintf(buffer, "Changing Baud rate from %lu to %lu\r\n", (SerialBaudRate == 0 ? 9600 : SerialBaudRate), baud);
		Usart0TransmitStr(buffer);
		SerialBaudRate = baud;
		Usart0ChangeBaudRate(baud);	
	}
}

void DeviceClass::SetSerialEchoEnabled(bool echoEnabled)
{
	DevSettingsNeedSaving = (SerialEchoEnabled != echoEnabled);
	SerialEchoEnabled = echoEnabled;
}

// ##### Settings ####################################################################################################################

void DeviceClass::SaveSettings()
{
	DevSettingsEEPROMLayout_t settings;
	/* Collect setting from appropriate classes */
	settings.PS_Voltage = PowerSupply.Voltage;
	settings.PS_LoadImpedance = PowerSupply.LoadImpedance;
	settings.Screens_TabIndex = ScreenManager.TabIndex;
	settings.Screens_Inverted = ScreenManager.DisplayInverted;
	settings.Device_SerialBaudRate = SerialBaudRate;
	settings.Device_SerialEchoEnabled = SerialEchoEnabled;
	settings.DDS1_Frequency = DDS_Channel1.GetFrequency();
	settings.DDS1_SignalForm = DDS_Channel1.GetSignalForm();
	settings.DDS1_Amplitude = DDS_Channel1.GetAmplitude();
	settings.DDS1_Offset = DDS_Channel1.GetOffset();

	eeprom_write_block((const void*)&settings, (void*)&NonVolatileSettings, sizeof(DevSettingsEEPROMLayout_t));
	
	DevSettingsNeedSaving = false;
}

void DeviceClass::LoadSettings()
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
	
	SetSerialBaudRate(settings.Device_SerialBaudRate);
	SetSerialEchoEnabled(settings.Device_SerialEchoEnabled);
	
	DDS_Channel1.SetEnabled(false);
	DDS_Channel1.SetFrequency(settings.DDS1_Frequency);
	DDS_Channel1.SetSignalForm(settings.DDS1_SignalForm);
	DDS_Channel1.SetAmplitude(settings.DDS1_Amplitude);
	DDS_Channel1.SetOffset(settings.DDS1_Offset);
	DDS_Channel1.UpdateWaveTable();
	
	DevSettingsNeedSaving = false;
}

void DeviceClass::ResetDevice()
{
	PowerSupply.Voltage = 5;
	PowerSupply.OutputEnabled = false;
	PowerSupply.LoadImpedance = 1000000;
	PowerSupply.UpdateOutput();
	ScreenManager.TabIndex = 0;
	ScreenManager.SetDisplayEnabled(true);
	ScreenManager.SetDisplayInverted(false);
	DDS_Channel1.SetEnabled(false);
	DDS_Channel1.SetFrequency(1000);
	DDS_Channel1.SetSignalForm(SINE);
	DDS_Channel1.SetAmplitude(10);
	DDS_Channel1.SetOffset(0);
	
	SaveSettings();
}
