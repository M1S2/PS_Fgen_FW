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
#include "Outputs/Channel.h"
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
	float PS_VOLT;
	float PS_CURR;
	float PS_POWER;
	float DMM1;
	float DMM2;
}DeviceVoltages_t;

/* This structure is only used internally to store to / read from EEPROM */
typedef struct DevSettingsEEPROMLayout
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
	//bool DDS1_Enabled;
}DevSettingsEEPROMLayout_t;



class DeviceClass
{
	public:	
		Channel Channels[NUM_OUTPUT_CHANNELS];			// !!! Channels must be initialized before ScreenManager, therefore it is necessary that the Channels are placed in front of the ScreenManager. Otherwise uninitialized channel limits are used in the screen controls !!!
	
		DeviceControlStates_t DeviceControlState;
		DeviceVoltages_t DeviceVoltages;

		UserInputHandlerClass UserInputHandler;
		ScreenManagerClass ScreenManager;

		uint32_t SerialBaudRate;
		bool SerialEchoEnabled;
		
		bool DevSettingsNeedSaving;
	
		DeviceClass();
		
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