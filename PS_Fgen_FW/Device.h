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
	DEV_CTRL_RWLOCK,
	NUM_DEV_CTRL_ELEMENTS		// The last element is used to determine the number of elements in the enumeration
}DeviceControlStates_t; 
extern const char* DeviceControlStateNames[NUM_DEV_CTRL_ELEMENTS];

typedef enum DevicePowerUpOutputEnabledStates
{
	DEV_POWERUP_OUTPUTS_OFF,
	DEV_POWERUP_OUTPUTS_LASTSTATE,
	DEV_POWERUP_OUTPUTS_ON,
	NUM_DEV_POWERUP_ELEMENTS		// The last element is used to determine the number of elements in the enumeration
}DevicePowerUpOutputEnabledStates_t;
extern const char* DevicePowerUpOutputEnabledStateNames[NUM_DEV_POWERUP_ELEMENTS];

typedef enum DeviceBaudRates
{
	DEV_BAUD_110,
	DEV_BAUD_150,
	DEV_BAUD_300,
	DEV_BAUD_1200,
	DEV_BAUD_2400,
	DEV_BAUD_4800,
	DEV_BAUD_9600,
	DEV_BAUD_19200,
	DEV_BAUD_38400,
	DEV_BAUD_57600,
	NUM_DEV_BAUD_ELEMENTS		// The last element is used to determine the number of elements in the enumeration
}DeviceBaudRates_t;
extern const char* DeviceBaudRateNames[NUM_DEV_BAUD_ELEMENTS];
uint32_t DeviceBaudRateEnumToNumber(DeviceBaudRates_t baudRateEnum);

typedef struct DeviceVoltagesStruct
{
	float ATX_3V3;
	float ATX_5V;
	float ATX_12V;
	float ATX_12V_NEG;
}DeviceVoltages_t;

typedef struct DeviceCalibrationFactors
{
	float Cal_RefVoltage;	// AVR VCC reference voltage in V
	float Cal_ATX_5V;		// Correction factor for the ATX 5V measurement
	float Cal_ATX_3V3;		// Correction factor for the ATX 3V3 measurement
	float Cal_ATX_12V;		// Correction factor for the ATX 12V measurement
	float Cal_ATX_12V_NEG;	// Correction factor for the ATX -12V measurement
	float Cal_DMM1;			// Correction factor for the DMM1 measurement
	float Cal_DMM2;			// Correction factor for the DMM2 measurement
	float Cal_PS_VOLT;		// Correction factor for the PS_VOLT measurement
	float Cal_DDS_FREQ;		// Correction factor for the DDS frequency setting
}DeviceCalibrationFactors_t;

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

	uint8_t Screens_Inverted;
	
	DeviceBaudRates_t Device_SerialBaudRate;
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
	
	DevicePowerUpOutputEnabledStates_t PowerOnOutputsState;
	
	DeviceCalibrationFactors_t CalibrationFactors;
	
}DevSettingsEEPROMLayout_t;

#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
	/* This structure is only used internally to store user waveform data to / read from EEPROM */
	typedef struct DevSettingsUserDDSWaveformEEPROMLayout
	{
		uint16_t DDS1_UserWaveTable[(1 << DDS_QUANTIZER_BITS)];				// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
		uint16_t DDS2_UserWaveTable[(1 << DDS_QUANTIZER_BITS)];				// Left shift to replace pow(2, DDS_QUANTIZER_BITS)
	}DevSettingsUserDDSWaveformEEPROMLayout_t;
#endif

class DeviceClass
{			
	private:
		bool _settingsChanged;
		
	public:	
		PS_Channel PsChannel;
		DDS_Channel DdsChannel1;
		DDS_Channel DdsChannel2;
		DMM_Channel DmmChannel1;
		DMM_Channel DmmChannel2;
		Channel* Channels[NUM_CHANNELS];			// !!! Channels must be initialized before ScreenManager, therefore it is necessary that the Channels are initialized with the initializer list. Otherwise uninitialized channel limits are used in the screen controls !!!
		
		uint8_t SelectedScpiChannelIndex;
		
		DevicePowerUpOutputEnabledStates_t PowerOnOutputsState;
		
		DeviceControlStates_t DeviceControlState;
		
		DeviceVoltages_t DeviceVoltages;
		DeviceCalibrationFactors_t CalibrationFactors;

		UserInputHandlerClass UserInputHandler;
		ScreenManagerClass ScreenManager;

		DeviceBaudRates_t SerialBaudRate;
		bool SerialEchoEnabled;
	
		uint16_t TimeCounter_KeyPolling_ms;
		uint16_t TimeCounter_ScreenRedraw_ms;
		uint16_t TimeCounter_AutoSave_ms;
	
		DeviceClass();
		void Init();
		void InitDeviceTimer();
		
		void DeviceMainLoop();
		
		void DeviceTimerTickISR(uint16_t currentPeriod_ms);
		
		void SetSettingsChanged(bool settingsChanged);
		
		void SaveSettings();
		void LoadSettings();
		void ResetDevice();
		
		#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
			void SaveSettingsDDSUserWaveforms();
			void LoadSettingsDDSUserWaveforms();
		#endif
		
		void SetDeviceControlState(DeviceControlStates_t controlState);
		bool IsUserInputLocked();
		void UpdateControlStateOnUserInput();
		
		void SetSerialBaudRate(DeviceBaudRates_t baud);
		void SetSerialEchoEnabled(bool echoEnabled);
};

extern DeviceClass Device;

#endif /* DEVICE_H_ */