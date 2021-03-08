/*
 * Device.h
 *
 * Created: 06.12.2020 14:44:34
 *  Author: V17
 */ 


#ifndef DEVICE_H_
#define DEVICE_H_

//"Use the Save EEPROM fusebit so it wont get deleted every time you program the mcu."
//https://www.avrfreaks.net/forum/tut-c-using-eeprom-memory-avr-gcc
#include <avr/eeprom.h>

#include <avr/io.h>
#include <stdbool.h>
#include "UserInputHandler/UserInputHandler.h"
#include "Screens/ScreenManager.h"
#include "Channels/Channel.h"
#include "Channels/DDS_Channel.h"
#include "Channels/PS_Channel.h"
#include "Channels/DMM_Channel.h"
#include "Configuration.h"

typedef enum DeviceControlStates
{
	DEV_CTRL_LOCAL,
	DEV_CTRL_REMOTE,
	DEV_CTRL_RWLOCK
}DeviceControlStates_t; 

typedef struct DeviceVoltagesStruct
{
	float ATX_3V3;
	float ATX_5V;
	float ATX_12V;
	float ATX_12V_NEG;
}DeviceVoltages_t;

/* This structure is only used internally to store to / read from EEPROM */
typedef struct DevSettingsEEPROMLayout
{
	float PS_Voltage;
	float PS_Current;
	bool PS_Enabled;
	uint8_t PS_OvpLevel;
	bool PS_OvpState;
	float PS_OvpDelay;
	uint8_t PS_OcpLevel;
	bool PS_OcpState;
	float PS_OcpDelay;
	float PS_OppLevel;
	bool PS_OppState;
	float PS_OppDelay;
	
	uint8_t Screens_TabIndex;
	uint8_t Screens_Inverted;
	
	uint32_t Device_SerialBaudRate;
	bool Device_SerialEchoEnabled;
	
	float DDS1_Frequency;
	SignalForms_t DDS1_SignalForm;
	float DDS1_Amplitude;
	float DDS1_Offset;
	bool DDS1_Enabled;
	
	float DDS2_Frequency;
	SignalForms_t DDS2_SignalForm;
	float DDS2_Amplitude;
	float DDS2_Offset;
	bool DDS2_Enabled;
	
	bool PowerOnOutputsDisabled;
}DevSettingsEEPROMLayout_t;


class DeviceClass
{			
	public:	
		PS_Channel PsChannel;
		DDS_Channel DdsChannel1;
		DDS_Channel DdsChannel2;
		DMM_Channel DmmChannel1;
		DMM_Channel DmmChannel2;
		Channel* Channels[NUM_CHANNELS];			// !!! Channels must be initialized before ScreenManager, therefore it is necessary that the Channels are initialized with the initializer list. Otherwise uninitialized channel limits are used in the screen controls !!!
		
		uint8_t SelectedScpiChannelIndex;
		
		bool PowerOnOutputsDisabled;				// true -> outputs are disabled on power on; false -> last output states are restored on power on
		
		DeviceControlStates_t DeviceControlState;
		DeviceVoltages_t DeviceVoltages;

		UserInputHandlerClass UserInputHandler;
		ScreenManagerClass ScreenManager;

		uint32_t SerialBaudRate;
		bool SerialEchoEnabled;
		
		bool DevSettingsNeedSaving;
	
		uint16_t TimeCounter_KeyPolling_ms;
		uint16_t TimeCounter_ScreenRedraw_ms;
		uint16_t TimeCounter_AutoSave_ms;
	
		DeviceClass();
		void Init();
		void InitDeviceTimer();
		
		void DeviceMainLoop();
		
		void DeviceTimerTickISR(uint16_t currentPeriod_ms);
		
		void SaveSettings();
		void LoadSettings();
		void ResetDevice();
		
		bool IsUserInputLocked();
		void UpdateControlStateOnUserInput();
		
		void SetSerialBaudRate(uint32_t baud);
		void SetSerialEchoEnabled(bool echoEnabled);
};

extern DeviceClass Device;

#endif /* DEVICE_H_ */