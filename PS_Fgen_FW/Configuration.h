/*
 * Configuration.h
 *
 * Created: 17.12.2020 19:47:56
 *  Author: V17
 */ 

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "Channels/DDS_Channel.h"
#include "Channels/PS_Channel.h"

/***** Version ************************************************/
#define VERSION							"v0.0.0"				// Software version. This should be incremented with every release.

/***** CPU Info ***********************************************/
#define CPU_TYPE						"ATMEGA1284P"			// Type of the controller
#define CPU_BOARD_VERSION				"v1.2"					// Version of the controller board

/***** Timers *************************************************/
#define DEVICE_TIMER_TICK_INTERVAL_MS	250						// Tick interval of the DeviceTimer in ms. The DeviceTimer is used to e.g. periodically read the keys and the encoder.
#define KEY_POLLING_DELAY_MS			250						// Time period for key polling (keys and encoder button). This must be higher than the DeviceTimer tick interval because it's derived from this device timer.
#define SCREEN_REDRAW_DELAY_MS			500						// Time period for screen redraw. This must be higher than the DeviceTimer tick interval because it's derived from this device timer.
#define SETTINGS_AUTOSAVE_DELAY_MS		60000					// Milliseconds between automatic saves of the device settings
#define SPLASHSCREEN_DELAY_MS			3000					// Time that the splash screen is shown in milliseconds

/***** SCPI ***************************************************/
//#define SCPI_ENABLED											// Comment this line out to disable the SCPI parser
#define SCPI_INPUT_BUFFER_LENGTH		64						// Length of the SCPI input buffer. If this buffer overflows, there will be an error message.
#define SCPI_ERROR_QUEUE_SIZE			4						// Maximum number of errors in the error queue. If this queue overflows, the last error message will be replaced by an overflow error message.
#define SCPI_IDN_MANUFACTURER			"Markus Scheich"		// Manufacturer info used in *IDN? query
#define SCPI_IDN_MODEL					"PS_Fgen"				// Model info used in *IDN? query
#define SCPI_IDN_SERIAL_NUMBER			"01"					// Serial number used in *IDN? query
#define SCPI_IDN_SOFTWARE_REVISION		VERSION					// Software version used in *IDN? query
#define SCPI_SYSTEM_CAPABILITIES		"(DCPSUPPLY WITH MEASURE|FUNCGEN|FUNCGEN)"		// All capabilites of the system					

/***** Screens ************************************************/
#define SCREEN_TAB_WIDTH				32						// Width in pixels of the Screen Tab control on the left side
#define SPLASHSCREEN_ENABLED									// Comment this line out to disable the Splashscreen

/***** UserInputHandler ***************************************/
#define USERINPUT_QUEUE_LENGTH			64						// Length of the queue used to buffer the user inputs (keys, encoder, Usart)

/***** Channels ***********************************************/
#define NUM_CHANNELS					5						// Number of channels

#define PS_INTERNAL_IMPEDANCE			5.7						// Internal impedance in Ohm of the PowerSupply output
#define PS_MIN_AMPLITUDE				0						// Minimum allowed power supply amplitude setting
#define PS_MAX_AMPLITUDE				10						// Maximum allowed power supply amplitude setting
#define PS_MIN_CURRENT					0						// Minimum allowed power supply current setting
#define PS_MAX_CURRENT					2						// Maximum allowed power supply current setting
#define PS_MIN_OVP_LEVEL_PERCENTAGE		90						// Minimum allowed power supply OVP level setting
#define PS_MAX_OVP_LEVEL_PERCENTAGE		200						// Maximum allowed power supply OVP level setting
#define PS_MIN_OVP_DELAY				0						// Minimum allowed power supply OVP delay setting
#define PS_MAX_OVP_DELAY				20						// Maximum allowed power supply OVP delay setting
#define PS_MIN_OCP_LEVEL_PERCENTAGE		PS_MIN_OVP_LEVEL_PERCENTAGE		// Minimum allowed power supply OCP level setting
#define PS_MAX_OCP_LEVEL_PERCENTAGE		PS_MAX_OVP_LEVEL_PERCENTAGE		// Maximum allowed power supply OCP level setting
#define PS_MIN_OCP_DELAY				PS_MIN_OVP_DELAY		// Minimum allowed power supply OCP delay setting
#define PS_MAX_OCP_DELAY				PS_MAX_OVP_DELAY		// Maximum allowed power supply OCP delay setting
#define PS_MIN_OPP_LEVEL				0						// Minimum allowed power supply OCP level setting
#define PS_MAX_OPP_LEVEL				(PS_MAX_AMPLITUDE * PS_MAX_CURRENT)		// Maximum allowed power supply OCP level setting
#define PS_MIN_OPP_DELAY				0						// Minimum allowed power supply OPP delay setting
#define PS_MAX_OPP_DELAY				65						// Maximum allowed power supply OPP delay setting

#define PS_VOLT_PID_P					0.2f					// Proportional part of the voltage PID regulator
#define PS_VOLT_PID_I					3.0f					// Integral part of the voltage PID regulator
#define PS_VOLT_PID_D					0						// Differential part of the voltage PID regulator

#define DDS_MIN_FREQ					0.001f					// Minimum allowed DDS frequency setting
#define DDS_MAX_FREQ					20000  					// Maximum allowed DDS frequency setting
#define DDS_MIN_AMPLITUDE				0						// Minimum allowed DDS amplitude setting
#define DDS_MAX_AMPLITUDE				20						// Maximum allowed DDS amplitude setting
#define DDS_MIN_OFFSET					-10						// Minimum allowed DDS offset setting
#define DDS_MAX_OFFSET					10						// Maximum allowed DDS offset setting

#endif /* CONFIGURATION_H_ */