/**
 * @file	Device.h
 * @date	06.12.2020 14:44:34
 * @author	Markus Scheich
 * @brief	Containing the main device class.
 */ 

#ifndef DEVICE_H_
#define DEVICE_H_

#include <Arduino.h>
#include <SPI.h>

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

/**
 * Available device control states.
 * Enumeration with all different device control states.
 */
typedef enum DeviceControlStates
{
	DEV_CTRL_LOCAL,				/**< Device is in local state. Each key press is processed directly. */
	DEV_CTRL_REMOTE,			/**< Device is in remote state. The first key press is ignored and brings the device back to local state. */
	DEV_CTRL_RWLOCK,			/**< Device is in RW lock state. All keys are ignored. The user must actively change back to local state via SCPI commands to reactivate the keys. */
	NUM_DEV_CTRL_ELEMENTS		/**< The last element is used to determine the number of elements in the enumeration */
}DeviceControlStates_t; 
extern const char* DeviceControlStateNames[NUM_DEV_CTRL_ELEMENTS];		/**< Array holding the names for all available device control states. */

/**
 * Available device power up output states.
 * Enumeration with all available output states on power up.
 */
typedef enum DevicePowerUpOutputEnabledStates
{
	DEV_POWERUP_OUTPUTS_OFF,		/**< Switch off all outputs on power up. */
	DEV_POWERUP_OUTPUTS_LASTSTATE,	/**< Switch all outputs to their last saved states on power up. */
	DEV_POWERUP_OUTPUTS_ON,			/**< Switch on all outputs on power up. */
	NUM_DEV_POWERUP_ELEMENTS		/**< The last element is used to determine the number of elements in the enumeration */
}DevicePowerUpOutputEnabledStates_t;
extern const char* DevicePowerUpOutputEnabledStateNames[NUM_DEV_POWERUP_ELEMENTS];	/**< Array holding the names for all available output power up states. */

/**
 * Structure containing all device ATX voltages.
 */
typedef struct DeviceVoltagesStruct
{
	float ATX_3V3;			/**< Voltage of the 3.3V ATX line */
	float ATX_5V;			/**< Voltage of the 5V ATX line */
}DeviceVoltages_t;

/**
 * Structure containing all device calibration factors.
 */
typedef struct DeviceCalibrationFactors
{
	float Cal_RefVoltage;		/**< AVR VCC reference voltage in V */
	float Cal_ATX_5V;			/**< Correction factor for the ATX 5V measurement */
	float Cal_ATX_3V3;			/**< Correction factor for the ATX 3V3 measurement */
	float Cal_DMM1;				/**< Correction factor for the DMM1 measurement */
	float Cal_DMM2;				/**< Correction factor for the DMM2 measurement */
	float Cal_PS_VOLT;			/**< Correction factor for the PS_VOLT measurement */
	float Cal_PS_CURR;			/**< Correction factor for the PS_CURR measurement */
	float Cal_PS_CURR_OFFSET;	/**< Correction factor for the PS_CURR offset measurement */
	float Cal_DDS_FREQ;			/**< Correction factor for the DDS frequency setting */

	bool Cal_Valid;				/**< If true, the calibration factors above are valid. Otherwise they were not measure before. */
}DeviceCalibrationFactors_t;

/**
 * Structure used internally to store to / read from EEPROM (except user waveforms and calibration factors)
 */
typedef struct DevSettingsEEPROMLayout
{
	float PS_Voltage;											/**< Power supply channel voltage */
	bool PS_Enabled;											/**< Power supply channel output enabled */
	uint8_t PS_OvpLevel;										/**< Power supply channel over voltage protection level */
	bool PS_OvpState;											/**< Power supply channel over voltage protection state */
	float PS_OvpDelay;											/**< Power supply channel over voltage protection delay */
	float PS_OcpLevel;											/**< Power supply channel over current protection level */
	bool PS_OcpState;											/**< Power supply channel over current protection state */
	float PS_OcpDelay;											/**< Power supply channel over current protection delay */
	float PS_OppLevel;											/**< Power supply channel over power protection level */
	bool PS_OppState;											/**< Power supply channel over power protection state */
	float PS_OppDelay;											/**< Power supply channel over power protection delay */
	PsRegulationModes_t PS_RegulationMode;						/**< Power supply channel regulation mode */

	float DDS1_Frequency;										/**< DDS channel 1 frequency */
	SignalForms_t DDS1_SignalForm;								/**< DDS channel 1 signal form */
	float DDS1_Amplitude;										/**< DDS channel 1 amplitude */
	float DDS1_Offset;											/**< DDS channel 1 offset */
	bool DDS1_Enabled;											/**< DDS channel 1 output enabled */
	float DDS1_PWM_Value;										/**< DDS channel 1 PWM value (only used for PWM SignalForm)*/
	
	float DDS2_Frequency;										/**< DDS channel 2 frequency */
	SignalForms_t DDS2_SignalForm;								/**< DDS channel 2 signal form */
	float DDS2_Amplitude;										/**< DDS channel 2 amplitude */
	float DDS2_Offset;											/**< DDS channel 2 offset */
	bool DDS2_Enabled;											/**< DDS channel 2 output enabled */
	float DDS2_PWM_Value;										/**< DDS channel 2 PWM value (only used for PWM SignalForm)*/
	
	DevicePowerUpOutputEnabledStates_t PowerOnOutputsState;		/**< Power up output state (state of all outputs on power up) */
		
}DevSettingsEEPROMLayout_t;

#if defined DDS_USER_DEFINED_WAVEFORMS_ENABLED && defined DDS_SUBSYSTEM_ENABLED
	/**
	  * Structure used internally to store user waveform data to / read from EEPROM 
	  */
	typedef struct DevSettingsUserDDSWaveformEEPROMLayout
	{
		uint16_t DDS1_UserWaveTable[(1 << DDS_QUANTIZER_BITS)];		/**< User waveform for DDS channel 1 */
		uint16_t DDS2_UserWaveTable[(1 << DDS_QUANTIZER_BITS)];		/**< User waveform for DDS channel 2 */
	}DevSettingsUserDDSWaveformEEPROMLayout_t;
#endif


/**
 * Class holding all relevant sub system objects and settings.
 */
class DeviceClass
{			
	private:
		bool _settingsChanged;										/**< Variable used to keep track if all settings are changed (true if no unsaved settings exist; otherwise false) */
		
	public:	
		#ifdef PS_SUBSYSTEM_ENABLED
			PS_Channel PsChannel;									/**< Power supply channel object */
		#endif
		#ifdef DDS_SUBSYSTEM_ENABLED
			DDS_Channel DdsChannel1;								/**< DDS channel object 1 */
			DDS_Channel DdsChannel2;								/**< DDS channel object 2 */
		#endif
		#ifdef MEASURE_SUBSYSTEM_ENABLED
			DMM_Channel DmmChannel1;								/**< DMM channel object 1 */
			DMM_Channel DmmChannel2;								/**< DMM channel object 2 */
		#endif
		Channel* Channels[NUM_CHANNELS];							/**< Array with pointers to all channel objects above */
		
		uint8_t SelectedScpiChannelIndex;							/**< Index of the channel selected by the "INSTrument[:SELect]" SCPI command */
		
		DevicePowerUpOutputEnabledStates_t PowerOnOutputsState;		/**< Power up output state (state of all outputs on power up) */
		
		DeviceControlStates_t DeviceControlState;					/**< Control state of the device (LOC, REM, RWL) */
		
		DeviceVoltages_t DeviceVoltages;							/**< Measured device ATX voltages */
		DeviceCalibrationFactors_t CalibrationFactors;				/**< Loaded calibration factors used for device operation */

		UserInputHandlerClass UserInputHandler;						/**< UserInputHandler object used for key and Usart enqueueing and dequeueing */
		ScreenManagerClass ScreenManager;							/**< Object for all screen drawing related stuff */

		uint16_t TimeCounter_KeyPolling_ms;							/**< Variable used for measuring the time to the next key polling */
		uint16_t TimeCounter_AutoSave_ms;							/**< Variable used for measuring the time to the next auto save */
		uint16_t TimeCounter_PowerSupplyChannelRegulation_ms;		/**< Variable used for measuring the time to the next power supply regulation loop */
	
		DeviceClass();												/**< Constructor of the DeviceClass */
		void Init();												/**< Initialize all sub systems of the device */
		void InitDeviceTimer();										/**< Initialize the device timer */
		
		void DeviceMainLoop();										/**< This method is called from the main() loop continuously */
		
		/**
		 * This method is called periodically by the device timer ISR and runs in the ISR context.
		 * It is used for key polling and forwarding the timer tick to other sub systems.
		 */
		void DeviceTimerTickISR();
		
		/**
		 * Change the value of the _settingsChanged variable and update the settings changed indicator (star in the upper right corner)
		 * @param settingsChanged Are all settings changed (true if no unsaved settings exist; otherwise false)
		 */
		void SetSettingsChanged(bool settingsChanged);
		
		/** Save all settings (except user waveforms and calibration factors) to the EEPROM */
		void SaveSettings();		
										
		/** Load all settings (except user waveforms and calibration factors) from the EEPROM */
		void LoadSettings();		
		
		/** Reset all settings (except user waveforms and calibration factors) and save them to the EEPROM */								
		void ResetDevice();					
		
		/** Reset all calibration settings and save them to the EEPROM */								
		void ResetDeviceCalibration();		

		/** Write all calibration factor values to the serial output. */
		void ReportCalibrationFactors();
		
		/** Coerce all calibration factors to valid values if neccessary. */
		void CoerceCalibrationFactors();
		
		/** Save the calibration factors from the EEPROM */						
		void SaveSettingsCalibrationFactors();		
		
		/** Load the calibration factors from the EEPROM */				
		void LoadSettingsCalibrationFactors();						
		
		#if defined DDS_USER_DEFINED_WAVEFORMS_ENABLED && defined DDS_SUBSYSTEM_ENABLED
			/** Save the user waveforms to the EEPROM */
			void SaveSettingsDDSUserWaveforms();
			
			/** Load the user waveforms from the EEPROM */					
			void LoadSettingsDDSUserWaveforms();					
		#endif
		
		/**
		 * Set the device control state to the given value.
		 * @param controlState New control state of the device (LOC, REM, RWL)
		 */
		void SetDeviceControlState(DeviceControlStates_t controlState);
		
		/**
		 * Check if the user input is locked (device control state is not LOC).
		 * @return true if device control state is not LOC; otherwise false.
		 */
		bool IsUserInputLocked();
		
		/** Update the device control state after an user input occurs (only changes to LOC if it was REM) */
		void UpdateControlStateOnUserInput();
};

extern DeviceClass Device;			/**< Global DeviceClass instance used to access all device related methods and properties. */

#endif /* DEVICE_H_ */