/*
 * Device.cpp
 * Created: 06.12.2020 14:44:51
 * Author: Markus Scheich
 */ 

#include "Device.h"
#include "ADC/ADC.h"
#include "Spi/spi.h"
#include "SCPI/SCPI_Device.h"
#include "Channels/DAC_MCP492x.h"

#include <stdio.h>
#include <avr/interrupt.h>
#include <TimerOne.h>

DeviceClass Device;
DevSettingsEEPROMLayout_t EEMEM NonVolatileSettings;
DeviceCalibrationFactors_t EEMEM NonVolatileSettings_CalibrationFactors;
#if defined DDS_USER_DEFINED_WAVEFORMS_ENABLED && defined DDS_SUBSYSTEM_ENABLED
	DevSettingsUserDDSWaveformEEPROMLayout_t EEMEM NonVolatileSettings_DDSUserWaveforms;
#endif

const char* DeviceControlStateNames[] = { "LOC", "REM", "RWL" };
const char* DevicePowerUpOutputEnabledStateNames[] = { "OFF", "LAST", "ON" };

DeviceClass::DeviceClass() :
#ifdef PS_SUBSYSTEM_ENABLED
	PsChannel(PS_MIN_VOLTAGE, PS_MAX_VOLTAGE, PS_MIN_OVP_LEVEL_PERCENTAGE, PS_MAX_OVP_LEVEL_PERCENTAGE, PS_MIN_OVP_DELAY, PS_MAX_OVP_DELAY, PS_MIN_OCP_LEVEL, PS_MAX_OCP_LEVEL, PS_MIN_OCP_DELAY, PS_MAX_OCP_DELAY, PS_MIN_OPP_LEVEL, PS_MAX_OPP_LEVEL, PS_MIN_OPP_DELAY, PS_MAX_OPP_DELAY),
#endif
#ifdef DDS_SUBSYSTEM_ENABLED
	DdsChannel1(1, DDS_MIN_FREQ, DDS_MAX_FREQ, DDS_MIN_AMPLITUDE, DDS_MAX_AMPLITUDE, DDS_MIN_OFFSET, DDS_MAX_OFFSET),
	DdsChannel2(2, DDS_MIN_FREQ, DDS_MAX_FREQ, DDS_MIN_AMPLITUDE, DDS_MAX_AMPLITUDE, DDS_MIN_OFFSET, DDS_MAX_OFFSET),
#endif
#ifdef MEASURE_SUBSYSTEM_ENABLED
	DmmChannel1(),
	DmmChannel2(),
#endif
	Channels 
	{ 
		#ifdef PS_SUBSYSTEM_ENABLED
			&PsChannel, 
		#endif
		#ifdef DDS_SUBSYSTEM_ENABLED
			&DdsChannel1, 
			&DdsChannel2, 
		#endif
		#ifdef MEASURE_SUBSYSTEM_ENABLED
			&DmmChannel1, 
			&DmmChannel2 
		#endif
	}
{
	DeviceControlState = DEV_CTRL_LOCAL;
}

void DeviceClass::Init()
{
	cli();
	Pins_Init();
	SPI_Init();
	Encoder_Init();
	OnOffControls_Init();
	ADC_init();
	Serial.begin(SERIAL_BAUDRATE);
	InitDeviceTimer();
	sei();
	
	#ifdef DDS_SUBSYSTEM_ENABLED
		InitDDSTimer();
	#endif
	
	Serial.println("Power On");
	
	ADC_startConversion();
	
	ScreenManager.Init();
	
	LoadSettings();

	ScreenManager.UpdateVisibilities();
	
	#ifdef SCPI_ENABLED
		SCPI_Init_Device();
	#endif
}

void DeviceClass::DeviceMainLoop()
{	
	int numCharacters = Serial.available();
    for(int i = 0; i < numCharacters; i++)
    {
        char data = Serial.read();
		Device.UserInputHandler.EnqueueUsartInput(data);
    }

	UserInputHandler.ProcessInputs();
	
	if(TimeCounter_AutoSave_ms >= SETTINGS_AUTOSAVE_DELAY_MS)
	{
		TimeCounter_AutoSave_ms = 0;
		if(_settingsChanged)
		{
			SaveSettings();
		}
	}
	
	ScreenManager.DoDraw();
}

// ##### Timer #######################################################################################################################

void timer1_tick_isr()
{
	cli();
	Device.DeviceTimerTickISR();
	sei();
}

/* Initialize 16-bit Timer/Counter1 */
void DeviceClass::InitDeviceTimer()
{
	TimeCounter_KeyPolling_ms = 0;
	TimeCounter_AutoSave_ms = 0;
	
	Timer1.initialize(DEVICE_TIMER_TICK_INTERVAL_MS * 1000UL);
	Timer1.attachInterrupt(timer1_tick_isr);
}

void DeviceClass::DeviceTimerTickISR(void)
{	
	TimeCounter_KeyPolling_ms += DEVICE_TIMER_TICK_INTERVAL_MS;	
	if(TimeCounter_KeyPolling_ms >= KEY_POLLING_DELAY_MS)
	{
		TimeCounter_KeyPolling_ms = 0;
		Keys_t key = KeyPad_GetKeys();
		if(key != KEYNONE)
		{
			UserInputHandler.EnqueueKeyInput(key);
		}
		if(Encoder_IsButtonPressed())
		{
			UserInputHandler.EnqueueKeyInput(KEYOK);
		}
	}
	
	TimeCounter_AutoSave_ms += DEVICE_TIMER_TICK_INTERVAL_MS;			// AutoSave is handled in DeviceMainLoop()
	
	ScreenManager.DeviceTimerTickISR();
	
	#ifdef PS_SUBSYSTEM_ENABLED
		TimeCounter_PowerSupplyChannelRegulation_ms += DEVICE_TIMER_TICK_INTERVAL_MS;
		if(TimeCounter_PowerSupplyChannelRegulation_ms >= POWER_SUPPLY_REG_INTERVAL_MS)
		{	
			TimeCounter_PowerSupplyChannelRegulation_ms = 0;
			PsChannel.DoRegulationISR();
		}
	#endif
	
	// Get on/off button state
	if(OnOffControls_IsButtonChanged())
	{
		OnOffButtons_t button = OnOffControls_GetButton();
		UserInputHandler.EnqueueOnOffButtonInput(button);
	}
}

// ##### Device Control State ########################################################################################################

void DeviceClass::SetDeviceControlState(DeviceControlStates_t controlState)
{
	DeviceControlState = controlState;
	ScreenManager.ShowHideDeviceRWLMessage(DeviceControlState == DEV_CTRL_RWLOCK);
}

bool DeviceClass::IsUserInputLocked() 
{
	 return (DeviceControlState != DEV_CTRL_LOCAL); 
}

void DeviceClass::UpdateControlStateOnUserInput()
{
	switch(DeviceControlState)
	{
		case DEV_CTRL_LOCAL: /* Nothing to do here. Device is already in local state. */ break;
		case DEV_CTRL_REMOTE: SetDeviceControlState(DEV_CTRL_LOCAL); break;
		case DEV_CTRL_RWLOCK: /* Nothing to do here. It is only possible to return from state RWLock via SCPI command. */ break;
		default: break;
	}
}

// ##### Settings ####################################################################################################################

void DeviceClass::SetSettingsChanged(bool settingsChanged)
{
	_settingsChanged = settingsChanged;
	ScreenManager.UpdateSettingsChangedIndicator(settingsChanged);
}

void DeviceClass::ReportCalibrationFactors()
{
	Serial.println("The following calibration factors are " + CalibrationFactors.Cal_Valid ? "VALID" : "INVALID");
	Serial.printf("RefVoltage=%.3fV\r\n", CalibrationFactors.Cal_RefVoltage);
	Serial.printf("5V=%.3f\r\n", CalibrationFactors.Cal_ATX_5V);
	Serial.printf("3V3=%.3f\r\n", CalibrationFactors.Cal_ATX_3V3);
	Serial.printf("DMM1=%.3f\r\n", CalibrationFactors.Cal_DMM1);
	Serial.printf("DMM2=%.3f\r\n", CalibrationFactors.Cal_DMM2);
	Serial.printf("PS_VOLT=%.3f\r\n", CalibrationFactors.Cal_PS_VOLT);
	Serial.printf("PS_CURR=%.3f\r\n", CalibrationFactors.Cal_PS_CURR);
	Serial.printf("PS_CURR_OFFSET=%.5f\r\n", CalibrationFactors.Cal_PS_CURR_OFFSET);
	Serial.printf("DDS_FREQ=%.3f\r\n", CalibrationFactors.Cal_DDS_FREQ);
}

void DeviceClass::CoerceCalibrationFactors()
{		
	if(CalibrationFactors.Cal_RefVoltage < 0.1 || CalibrationFactors.Cal_RefVoltage > 10 || isnan(CalibrationFactors.Cal_RefVoltage)) { CalibrationFactors.Cal_RefVoltage = 5; }
	if(CalibrationFactors.Cal_ATX_3V3 < 0.1 || CalibrationFactors.Cal_ATX_3V3 > 10 || isnan(CalibrationFactors.Cal_ATX_3V3)) { CalibrationFactors.Cal_ATX_3V3 = 1; }
	if(CalibrationFactors.Cal_ATX_5V < 0.1 || CalibrationFactors.Cal_ATX_5V > 10 || isnan(CalibrationFactors.Cal_ATX_5V)) { CalibrationFactors.Cal_ATX_5V = 1; }
	if(CalibrationFactors.Cal_DMM1 < 0.1 || CalibrationFactors.Cal_DMM1 > 10 || isnan(CalibrationFactors.Cal_DMM1)) { CalibrationFactors.Cal_DMM1 = 1; }
	if(CalibrationFactors.Cal_DMM2 < 0.1 || CalibrationFactors.Cal_DMM2 > 10 || isnan(CalibrationFactors.Cal_DMM2)) { CalibrationFactors.Cal_DMM2 = 1; }
	if(CalibrationFactors.Cal_PS_VOLT < 0.1 || CalibrationFactors.Cal_PS_VOLT > 10 || isnan(CalibrationFactors.Cal_PS_VOLT)) { CalibrationFactors.Cal_PS_VOLT = 1; }
	if(CalibrationFactors.Cal_PS_CURR < 0.1 || CalibrationFactors.Cal_PS_CURR > 10 || isnan(CalibrationFactors.Cal_PS_CURR)) { CalibrationFactors.Cal_PS_CURR = 1; }
	if(abs(CalibrationFactors.Cal_PS_CURR_OFFSET) > 1 || isnan(CalibrationFactors.Cal_PS_CURR_OFFSET)) { CalibrationFactors.Cal_PS_CURR_OFFSET = 0; }
	if(CalibrationFactors.Cal_DDS_FREQ < 0.1 || CalibrationFactors.Cal_DDS_FREQ > 10 || isnan(CalibrationFactors.Cal_DDS_FREQ)) { CalibrationFactors.Cal_DDS_FREQ = 1; }
}

// -----------------------------------------------------------------------------------------------------------------------------------

void DeviceClass::SaveSettings()
{
	DevSettingsEEPROMLayout_t settings;
	/* Collect setting from appropriate classes */

	#ifdef PS_SUBSYSTEM_ENABLED
		settings.PS_Voltage = PsChannel.GetVoltage();
		settings.PS_Enabled = PsChannel.GetEnabled();
		settings.PS_OvpLevel = PsChannel.GetOvpLevel();
		settings.PS_OvpState = PsChannel.GetOvpState();
		settings.PS_OvpDelay = PsChannel.GetOvpDelay();
		settings.PS_OcpLevel = PsChannel.GetOcpLevel();
		settings.PS_OcpState = PsChannel.GetOcpState();
		settings.PS_OcpDelay = PsChannel.GetOcpDelay();
		settings.PS_OppLevel = PsChannel.GetOppLevel();
		settings.PS_OppState = PsChannel.GetOppState();
		settings.PS_OppDelay = PsChannel.GetOppDelay();
		settings.PS_RegulationMode = PsChannel.GetRegulationMode();
	#endif
	
	#ifdef DDS_SUBSYSTEM_ENABLED
		settings.DDS1_Frequency = DdsChannel1.GetFrequency();
		settings.DDS1_SignalForm = DdsChannel1.GetSignalForm();
		settings.DDS1_Amplitude = DdsChannel1.GetAmplitude();
		settings.DDS1_Offset = DdsChannel1.GetOffset();
		settings.DDS1_Enabled = DdsChannel1.GetEnabled();
		settings.DDS1_PWM_Value = DdsChannel1.GetPWMValue();

		settings.DDS2_Frequency = DdsChannel2.GetFrequency();
		settings.DDS2_SignalForm = DdsChannel2.GetSignalForm();
		settings.DDS2_Amplitude = DdsChannel2.GetAmplitude();
		settings.DDS2_Offset = DdsChannel2.GetOffset();
		settings.DDS2_Enabled = DdsChannel2.GetEnabled();
		settings.DDS2_PWM_Value = DdsChannel2.GetPWMValue();
	#endif
	
	settings.PowerOnOutputsState = PowerOnOutputsState;

	eeprom_write_block((const void*)&settings, (void*)&NonVolatileSettings, sizeof(DevSettingsEEPROMLayout_t));
	
	SetSettingsChanged(false);
}

void DeviceClass::SaveSettingsCalibrationFactors()
{
	CoerceCalibrationFactors();
	ScreenManager.UpdateCalibrationValidIndicator(CalibrationFactors.Cal_Valid);
	eeprom_write_block((const void*)&CalibrationFactors, (void*)&NonVolatileSettings_CalibrationFactors, sizeof(DeviceCalibrationFactors_t));
}

#if defined DDS_USER_DEFINED_WAVEFORMS_ENABLED && defined DDS_SUBSYSTEM_ENABLED
void DeviceClass::SaveSettingsDDSUserWaveforms()
{
	DevSettingsUserDDSWaveformEEPROMLayout_t settingsDDSUserWaveform;
	memcpy(settingsDDSUserWaveform.DDS1_UserWaveTable, DdsChannel1.UserWaveTable, (1 << DDS_QUANTIZER_BITS) * sizeof(uint16_t));
	memcpy(settingsDDSUserWaveform.DDS2_UserWaveTable, DdsChannel2.UserWaveTable, (1 << DDS_QUANTIZER_BITS) * sizeof(uint16_t));
	
	eeprom_write_block((const void*)&settingsDDSUserWaveform, (void*)&NonVolatileSettings_DDSUserWaveforms, sizeof(DevSettingsUserDDSWaveformEEPROMLayout_t));
}
#endif

// -----------------------------------------------------------------------------------------------------------------------------------

void DeviceClass::LoadSettings()
{
	DevSettingsEEPROMLayout_t settings;
	eeprom_read_block((void*)&settings, (const void*)&NonVolatileSettings, sizeof(DevSettingsEEPROMLayout_t));
	
	/* Assign Settings to appropriate classes */

	LoadSettingsCalibrationFactors();
	
	PowerOnOutputsState = settings.PowerOnOutputsState;
	
	#ifdef PS_SUBSYSTEM_ENABLED
		PsChannel.SetVoltage(isnan(settings.PS_Voltage) ? PsChannel.Voltage.Def : settings.PS_Voltage);
		PsChannel.SetOvpLevel(isnan(settings.PS_OvpLevel) ? PsChannel.OvpLevel.Def : settings.PS_OvpLevel);
		PsChannel.SetOvpState(isnan(settings.PS_OvpState) ? false : settings.PS_OvpState);
		PsChannel.SetOvpDelay(isnan(settings.PS_OvpDelay) ? PsChannel.OvpDelay.Def : settings.PS_OvpDelay);
		PsChannel.SetOcpLevel(isnan(settings.PS_OcpLevel) ? PsChannel.OcpLevel.Def : settings.PS_OcpLevel);
		PsChannel.SetOcpState(isnan(settings.PS_OcpState) ? false : settings.PS_OcpState);
		PsChannel.SetOcpDelay(isnan(settings.PS_OcpDelay) ? PsChannel.OcpDelay.Def : settings.PS_OcpDelay);
		PsChannel.SetOppLevel(isnan(settings.PS_OppLevel) ? PsChannel.OppLevel.Def : settings.PS_OppLevel);
		PsChannel.SetOppState(isnan(settings.PS_OppState) ? false : settings.PS_OppState);
		PsChannel.SetOppDelay(isnan(settings.PS_OppDelay) ? PsChannel.OppDelay.Def : settings.PS_OppDelay);
		PsChannel.SetRegulationMode(isnan(settings.PS_RegulationMode) ? PS_REG_MODE_CV : settings.PS_RegulationMode);
		PsChannel.SetEnabled(PowerOnOutputsState == DEV_POWERUP_OUTPUTS_OFF ? false : (PowerOnOutputsState == DEV_POWERUP_OUTPUTS_ON ? true : settings.PS_Enabled));
	#endif
	
	#ifdef DDS_SUBSYSTEM_ENABLED		
		DdsChannel1.SetFrequency(isnan(settings.DDS1_Frequency) ? DdsChannel1.Frequency.Def : settings.DDS1_Frequency);
		DdsChannel1.SetSignalForm(isnan(settings.DDS1_SignalForm) ? DdsChannel1.SignalForm.Def : settings.DDS1_SignalForm);
		DdsChannel1.SetAmplitude(isnan(settings.DDS1_Amplitude) ? DdsChannel1.Amplitude.Def : settings.DDS1_Amplitude);
		DdsChannel1.SetOffset(isnan(settings.DDS1_Offset) ? DdsChannel1.Offset.Def : settings.DDS1_Offset);
		DdsChannel1.SetPWMValue(isnan(settings.DDS1_PWM_Value) ? DdsChannel1.PWM_Value.Def : settings.DDS1_PWM_Value);
		DdsChannel1.UpdateOriginalWaveTable();
		DdsChannel1.UpdateWaveTable();
		DdsChannel1.SetEnabled(PowerOnOutputsState == DEV_POWERUP_OUTPUTS_OFF ? false : (PowerOnOutputsState == DEV_POWERUP_OUTPUTS_ON ? true : settings.DDS1_Enabled));
		
		DdsChannel2.SetFrequency(isnan(settings.DDS2_Frequency) ? DdsChannel2.Frequency.Def : settings.DDS2_Frequency);
		DdsChannel2.SetSignalForm(isnan(settings.DDS2_SignalForm) ? DdsChannel2.SignalForm.Def : settings.DDS2_SignalForm);
		DdsChannel2.SetAmplitude(isnan(settings.DDS2_Amplitude) ? DdsChannel2.Amplitude.Def : settings.DDS2_Amplitude);
		DdsChannel2.SetOffset(isnan(settings.DDS2_Offset) ? DdsChannel2.Offset.Def : settings.DDS2_Offset);
		DdsChannel2.SetPWMValue(isnan(settings.DDS2_PWM_Value) ? DdsChannel2.PWM_Value.Def : settings.DDS2_PWM_Value);
		DdsChannel2.UpdateOriginalWaveTable();
		DdsChannel2.UpdateWaveTable();
		DdsChannel2.SetEnabled(PowerOnOutputsState == DEV_POWERUP_OUTPUTS_OFF ? false : (PowerOnOutputsState == DEV_POWERUP_OUTPUTS_ON ? true : settings.DDS2_Enabled));
	#endif					
		
	SetSettingsChanged(false);
	
	#if defined DDS_USER_DEFINED_WAVEFORMS_ENABLED && defined DDS_SUBSYSTEM_ENABLED
		LoadSettingsDDSUserWaveforms();
	#endif
}

void DeviceClass::LoadSettingsCalibrationFactors()
{
	eeprom_read_block((void*)&CalibrationFactors, (const void*)&NonVolatileSettings_CalibrationFactors, sizeof(DeviceCalibrationFactors_t));
	CoerceCalibrationFactors();
	ReportCalibrationFactors();
	ScreenManager.UpdateCalibrationValidIndicator(CalibrationFactors.Cal_Valid);
}

#if defined DDS_USER_DEFINED_WAVEFORMS_ENABLED && defined DDS_SUBSYSTEM_ENABLED
void DeviceClass::LoadSettingsDDSUserWaveforms()
{
	DevSettingsUserDDSWaveformEEPROMLayout_t settingsDDSUserWaveform;
	eeprom_read_block((void*)&settingsDDSUserWaveform, (const void*)&NonVolatileSettings_DDSUserWaveforms, sizeof(DevSettingsUserDDSWaveformEEPROMLayout_t));
	
	memcpy(DdsChannel1.UserWaveTable, settingsDDSUserWaveform.DDS1_UserWaveTable, (1 << DDS_QUANTIZER_BITS) * sizeof(uint16_t));
	memcpy(DdsChannel2.UserWaveTable, settingsDDSUserWaveform.DDS2_UserWaveTable, (1 << DDS_QUANTIZER_BITS) * sizeof(uint16_t));	
}
#endif

// -----------------------------------------------------------------------------------------------------------------------------------

void DeviceClass::ResetDevice()
{	
	#ifdef PS_SUBSYSTEM_ENABLED
		PsChannel.SetVoltage(PsChannel.Voltage.Def);
		PsChannel.SetEnabled(false);
		PsChannel.SetOvpLevel(PsChannel.OvpLevel.Def);
		PsChannel.SetOvpState(PsChannel.OvpState.Def);
		PsChannel.SetOvpDelay(PsChannel.OvpDelay.Def);
		PsChannel.SetOcpLevel(PsChannel.OcpLevel.Def);
		PsChannel.SetOcpState(PsChannel.OcpState.Def);
		PsChannel.SetOcpDelay(PsChannel.OcpDelay.Def);	
		PsChannel.SetOppLevel(PsChannel.OppLevel.Def);
		PsChannel.SetOppState(PsChannel.OppState.Def);
		PsChannel.SetOppDelay(PsChannel.OppDelay.Def);
		PsChannel.SetRegulationMode(PS_REG_MODE_CV);
	#endif
	
	#ifdef DDS_SUBSYSTEM_ENABLED
		DdsChannel1.SetEnabled(false);
		DdsChannel1.SetFrequency(DdsChannel1.Frequency.Def);
		DdsChannel1.SetSignalForm(DdsChannel1.SignalForm.Def);
		DdsChannel1.SetAmplitude(DdsChannel1.Amplitude.Def);
		DdsChannel1.SetOffset(DdsChannel1.Offset.Def);
	
		DdsChannel2.SetEnabled(false);
		DdsChannel2.SetFrequency(DdsChannel2.Frequency.Def);
		DdsChannel2.SetSignalForm(DdsChannel2.SignalForm.Def);
		DdsChannel2.SetAmplitude(DdsChannel2.Amplitude.Def);
		DdsChannel2.SetOffset(DdsChannel2.Offset.Def);
	#endif
		
	PowerOnOutputsState = DEV_POWERUP_OUTPUTS_OFF;
	//Calibration factors are not resetted. A new calibration must be done to change the factors.
	
	SaveSettings();
}

void DeviceClass::ResetDeviceCalibration()
{
	CalibrationFactors.Cal_RefVoltage = 5;
	CalibrationFactors.Cal_ATX_3V3 = 1;
	CalibrationFactors.Cal_ATX_5V = 1;
	CalibrationFactors.Cal_DMM1 = 1;
	CalibrationFactors.Cal_DMM2 = 1;
	CalibrationFactors.Cal_PS_VOLT = 1;
	CalibrationFactors.Cal_PS_CURR = 1;
	CalibrationFactors.Cal_PS_CURR_OFFSET = 0;
	CalibrationFactors.Cal_DDS_FREQ = 1;
	CalibrationFactors.Cal_Valid = false;

	SaveSettingsCalibrationFactors();
}