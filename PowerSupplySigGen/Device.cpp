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

DeviceClass::DeviceClass() :
	PsChannel(PS_MIN_AMPLITUDE, PS_MAX_AMPLITUDE, PS_MIN_LOAD, PS_MAX_LOAD),
	DdsChannel1(DDS_MIN_FREQ, DDS_MAX_FREQ, DDS_MIN_AMPLITUDE, DDS_MAX_AMPLITUDE, DDS_MIN_OFFSET, DDS_MAX_OFFSET),
	DdsChannel2(DDS_MIN_FREQ, DDS_MAX_FREQ, DDS_MIN_AMPLITUDE, DDS_MAX_AMPLITUDE, DDS_MIN_OFFSET, DDS_MAX_OFFSET),
	DmmChannel1(),
	DmmChannel2(),
	Channels{ &PsChannel, &DdsChannel1, &DdsChannel2, &DmmChannel1, &DmmChannel2 }
{
	DeviceControlState = DEV_CTRL_LOCAL;
}

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
	settings.Screens_TabIndex = ScreenManager.TabIndex;
	settings.Screens_Inverted = ScreenManager.DisplayInverted;
	settings.Device_SerialBaudRate = SerialBaudRate;
	settings.Device_SerialEchoEnabled = SerialEchoEnabled;
	
	settings.PS_Voltage = PsChannel.GetAmplitude();
	settings.PS_LoadImpedance = PsChannel.GetLoadImpedance();
		
	settings.DDS1_Frequency = DdsChannel1.GetFrequency();
	settings.DDS1_SignalForm = DdsChannel1.GetSignalForm();
	settings.DDS1_Amplitude = DdsChannel1.GetAmplitude();
	settings.DDS1_Offset = DdsChannel1.GetOffset();

	eeprom_write_block((const void*)&settings, (void*)&NonVolatileSettings, sizeof(DevSettingsEEPROMLayout_t));
	
	DevSettingsNeedSaving = false;
}

void DeviceClass::LoadSettings()
{
	DevSettingsEEPROMLayout_t settings;
	eeprom_read_block((void*)&settings, (const void*)&NonVolatileSettings, sizeof(DevSettingsEEPROMLayout_t));
	
	/* Assign Settings to appropriate classes */
	
	ScreenManager.TabIndex = settings.Screens_TabIndex;
	ScreenManager.SetDisplayInverted(settings.Screens_Inverted);
	
	SetSerialBaudRate(settings.Device_SerialBaudRate);
	SetSerialEchoEnabled(settings.Device_SerialEchoEnabled);
	
	PsChannel.SetAmplitude(settings.PS_Voltage);
	PsChannel.SetEnabled(false);
	PsChannel.SetLoadImpedance(settings.PS_LoadImpedance);
		
	DdsChannel1.SetEnabled(false);
	DdsChannel1.SetFrequency(settings.DDS1_Frequency);
	DdsChannel1.SetSignalForm(settings.DDS1_SignalForm);
	DdsChannel1.SetAmplitude(settings.DDS1_Amplitude);
	DdsChannel1.SetOffset(settings.DDS1_Offset);
	
	DevSettingsNeedSaving = false;
}

void DeviceClass::ResetDevice()
{
	ScreenManager.TabIndex = 0;
	ScreenManager.SetDisplayEnabled(true);
	ScreenManager.SetDisplayInverted(false);
	
	PsChannel.SetAmplitude(5);
	PsChannel.SetEnabled(false);
	PsChannel.SetLoadImpedance(1000000);
	
	DdsChannel1.SetEnabled(false);
	DdsChannel1.SetFrequency(1000);
	DdsChannel1.SetSignalForm(SINE);
	DdsChannel1.SetAmplitude(10);
	DdsChannel1.SetOffset(0);
	
	SaveSettings();
}
