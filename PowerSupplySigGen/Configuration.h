/*
 * Configuration.h
 *
 * Created: 17.12.2020 19:47:56
 *  Author: V17
 */ 

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "Outputs/DDS_Channel.h"
#include "Outputs/PS_Channel.h"

#define AVR_VCC_REF						5.27					// AVR VCC Reference voltage in V

/***** Version ************************************************/
#define VERSION							"v0.0.0"				// Software version. This should be incremented with every release.

/***** CPU Info ***********************************************/
#define CPU_TYPE						"ATMEGA1284P"			// Type of the controller
#define CPU_BOARD_VERSION				"v1.2"					// Version of the controller board

/***** User Timer and Saving **********************************/
#define USER_TIMER_TICK_FREQ			5						// Tick frequency of the UserTimer in Hz. The UserTimer is used to periodically read the keys and the encoder.
#define SETTINGS_AUTOSAVE_DELAY_SEC		60						// Seconds between automatic saves of the device settings

/***** SCPI ***************************************************/
#define SCPI_ENABLED											// Comment this line out to disable the SCPI parser
#define SCPI_INPUT_BUFFER_LENGTH		256						// Length of the SCPI input buffer. If this buffer overflows, there will be an error message.
#define SCPI_ERROR_QUEUE_SIZE			17						// Maximum number of errors in the error queue. If this queue overflows, the last error message will be replaced by an overflow error message.
#define SCPI_IDN_MANUFACTURER			"Markus Scheich"		// Manufacturer info used in *IDN? query
#define SCPI_IDN_MODEL					"PowerSupplySigGen"		// Model info used in *IDN? query
#define SCPI_IDN_SERIAL_NUMBER			"01"					// Serial number used in *IDN? query
#define SCPI_IDN_SOFTWARE_REVISION		VERSION					// Software version used in *IDN? query

/***** Screens ************************************************/
#define NUM_SCREENS						5						// Number of screens.

#define SCREEN_TAB_WIDTH				32						// Width in pixels of the Screen Tab control on the left side
#define SCREEN_TAB_HEIGHT				12						// Height in pixels of the Screen Tab control on the left side
#define SCREEN_TAB_MARGIN				1						// Margin in pixels between the Screen Tabs on the left side
#define SCREEN_TAB_FONT_HEIGHT			8						// Font Height in pixels of the Screen Tabs text on the left side

#define MAX_SCREEN_USER_CONTROLS		5						// Maximum number of user controls allowed per Screen
#define MAX_SCREEN_USER_INDICATORS		5						// Maximum number of user indicators allowed per Screen
#define MESSAGE_MARGIN					5						// Margin of the message popup box (user messages or error messages) to the display outline
#define MAX_MESSAGE_LEN					256						// Maximum length of user or error messages

#define SPLASHSCREEN_ENABLED									// Comment this line out to disable the Splashscreen
#define SPLASHSCREEN_DELAY_SEC			3						// Time that the splash screen is shown in seconds

/***** UserControlsIndicators **********************************/
#define CONTROLS_FONT_HEIGHT			8						// Font Height in pixels of the text used in UserControls
#define CONTROLS_WIDTH					65						// Width in pixels of the UserControls
#define INDICATORS_FONT_HEIGHT			8						// Font Height in pixels of the text used in UserIndicators

/***** UserInputHandler ***************************************/
#define USERINPUT_QUEUE_LENGTH			256						// Length of the queue used to buffer the user inputs (keys, encoder, Usart)

/***** Channels ***********************************************/
#define NUM_OUTPUT_CHANNELS				3						// Number of output channels

#define PS_INTERNAL_IMPEDANCE			5.7						// Internal impedance in Ohm of the PowerSupply output
#define PS_MIN_AMPLITUDE				0						// Minimum allowed power supply amplitude setting
#define PS_MAX_AMPLITUDE				10						// Maximum allowed power supply amplitude setting
#define PS_MIN_LOAD						1						// Minimum allowed power supply load impedance setting
#define PS_MAX_LOAD						1000000					// Maximum allowed power supply load impedance setting

#define DDS_MIN_FREQ					1						// Minimum allowed DDS frequency setting
#define DDS_MAX_FREQ					1000					// Maximum allowed DDS frequency setting
#define DDS_MIN_AMPLITUDE				0						// Minimum allowed DDS amplitude setting
#define DDS_MAX_AMPLITUDE				20						// Maximum allowed DDS amplitude setting
#define DDS_MIN_OFFSET					-10						// Minimum allowed DDS offset setting
#define DDS_MAX_OFFSET					10						// Maximum allowed DDS offset setting

#endif /* CONFIGURATION_H_ */