/**
 * @file	Configuration.h
 * @date	17.12.2020 19:47:56
 * @author	Markus Scheich
 * @brief	Containing different defines for device configuration
 */ 

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

/***** Version ************************************************/
#define VERSION							"[NO_RELEASE]"			/**< Software version. This should be incremented with every release. */

/***** Timers *************************************************/
#define DEVICE_TIMER_TICK_INTERVAL_MS	100						/**< Tick interval of the DeviceTimer in ms. The DeviceTimer is used to e.g. periodically read the keys and the encoder. */
#define POWER_SUPPLY_REG_INTERVAL_MS	100						/**< Time period for power supply regulation. This must be higher than the DeviceTimer tick interval because it's derived from this device timer. */
#define KEY_POLLING_DELAY_MS			250						/**< Time period for key polling (keys and encoder button). This must be higher than the DeviceTimer tick interval because it's derived from this device timer. */
#define SCREEN_REDRAW_DELAY_MS			500						/**< Time period for screen redraw. This must be higher than the DeviceTimer tick interval because it's derived from this device timer. */
#define SETTINGS_AUTOSAVE_DELAY_MS		60000					/**< Milliseconds between automatic saves of the device settings */
#define SPLASHSCREEN_DELAY_MS			2000					/**< Time that the splash screen is shown in milliseconds */

/***** SCPI ***************************************************/
//#define SCPI_ENABLED											/**< Comment this line out to disable the SCPI parser */
#define SCPI_INPUT_BUFFER_LENGTH		64						/**< Length of the SCPI input buffer. If this buffer overflows, there will be an error message. */
#define SCPI_ERROR_QUEUE_SIZE			10						/**< Maximum number of errors in the error queue. If this queue overflows, the last error message will be replaced by an overflow error message. */
#define SCPI_IDN_MANUFACTURER			"Markus Scheich"		/**< Manufacturer info used in *IDN? query */
#define SCPI_IDN_MODEL					"PS_Fgen"				/**< Model info used in *IDN? query */
#define SCPI_IDN_SERIAL_NUMBER			"01"					/**< Serial number used in *IDN? query */
#define SCPI_IDN_SOFTWARE_REVISION		VERSION					/**< Software version used in *IDN? query */
#define SCPI_SYSTEM_CAPABILITIES		"(DCPSUPPLY WITH MEASURE|FUNCGEN|FUNCGEN)"		/**< All capabilites of the system */			

/***** Screens ************************************************/
#define SCREEN_TAB_WIDTH				70						/**< Width in pixels of the Screen Tab control on the left side */
#define SPLASHSCREEN_ENABLED									/**< Comment this line out to disable the Splashscreen */

/***** UserInputHandler ***************************************/
#define USERINPUT_QUEUE_LENGTH			64						/**< Length of the queue used to buffer the user inputs (keys, encoder, Usart) */

/***** Subsystems *********************************************/
#define PS_SUBSYSTEM_ENABLED									/**< Comment this line out to disable the power supply subsystem */
#define DDS_SUBSYSTEM_ENABLED									/**< Comment this line out to disable the direct digital synthesis subsystem */
#define MEASURE_SUBSYSTEM_ENABLED								/**< Comment this line out to disable the measurement (ATX voltages and digital multimeters) subsystem */

/***** Direct Digital Synthesis (DDS) *************************/
#define DDS_TICK_FREQ			15625							/**< Frequency at which the phase accumulator is updated. Choose a value here that delivers an integer value for the OCR2A register (  ((F_CPU / 1024) / DDS_TICK_FREQ) should be an integer value ) !!! */
#define DDS_PHASE_ACCU_BITS		32								/**< Number of bits used by the phase accumulator of the direct digital synthesis */
#define DDS_QUANTIZER_BITS		8								/**< Number of bits that are used to index into the look up tables */
#define DDS_DAC_BITS			12								/**< Number of bits used by the DAC */
#define DDS_SAMPLE_MAX			((1 << DDS_DAC_BITS) - 1)		/**< Maximum value that can be reached with the number of bits in DDS_DAC_BITS */
#define DDS_AMPLITUDE_MAX		20.0f							/**< Maximum supported amplitude at the DDS outputs */

/***** Channels ***********************************************/
#ifdef PS_SUBSYSTEM_ENABLED
	#define NUM_PS_CHANNELS				1						/**< Number of power supply output channels if the PS subsystem is enabled */
#else
	#define NUM_PS_CHANNELS				0						/**< Number of power supply output channels if the PS subsystem is disabled */
#endif

#ifdef DDS_SUBSYSTEM_ENABLED
	#define NUM_DDS_CHANNELS			2						/**< Number of direct digital synthesis output channels if the DDS subsystem is enabled */
#else
	#define NUM_DDS_CHANNELS			0						/**< Number of direct digital synthesis output channels if the DDS subsystem is disabled */
#endif

#ifdef MEASURE_SUBSYSTEM_ENABLED
	#define NUM_MEASURE_CHANNELS		2						/**< Number of digital multimeter output channels if the DDS subsystem is enabled */
#else
	#define NUM_MEASURE_CHANNELS		0						/**< Number of digital multimeter output channels if the DDS subsystem is disabled */
#endif

#define NUM_CHANNELS					NUM_PS_CHANNELS + NUM_DDS_CHANNELS + NUM_MEASURE_CHANNELS	/**< Number of channels (this is the sum of NUM_PS_CHANNELS, NUM_DDS_CHANNELS and NUM_MEASURE_CHANNELS) */

#define PS_INTERNAL_IMPEDANCE			5.7						/**< Internal impedance in Ohm of the PowerSupply output */
#define PS_MIN_VOLTAGE					0						/**< Minimum allowed power supply voltage setting */
#define PS_MAX_VOLTAGE					10						/**< Maximum allowed power supply voltage setting */
#define PS_MAX_CURRENT					2						/**< Maximum allowed power supply current setting */
#define PS_MIN_OVP_LEVEL_PERCENTAGE		90						/**< Minimum allowed power supply OVP level setting (in percent of the configured voltage) */
#define PS_MAX_OVP_LEVEL_PERCENTAGE		200						/**< Maximum allowed power supply OVP level setting (in percent of the configured voltage) */
#define PS_MIN_OVP_DELAY				0						/**< Minimum allowed power supply OVP delay setting */
#define PS_MAX_OVP_DELAY				20						/**< Maximum allowed power supply OVP delay setting */
#define PS_MIN_OCP_LEVEL				0						/**< Minimum allowed power supply OCP level setting */
#define PS_MAX_OCP_LEVEL				3						/**< Maximum allowed power supply OCP level setting */
#define PS_MIN_OCP_DELAY				PS_MIN_OVP_DELAY		/**< Minimum allowed power supply OCP delay setting */
#define PS_MAX_OCP_DELAY				PS_MAX_OVP_DELAY		/**< Maximum allowed power supply OCP delay setting */
#define PS_MIN_OPP_LEVEL				0						/**< Minimum allowed power supply OCP level setting */
#define PS_MAX_OPP_LEVEL				(PS_MAX_VOLTAGE * PS_MAX_OCP_LEVEL)		/**< Maximum allowed power supply OCP level setting */
#define PS_MIN_OPP_DELAY				0						/**< Minimum allowed power supply OPP delay setting */
#define PS_MAX_OPP_DELAY				65						/**< Maximum allowed power supply OPP delay setting */

#define PS_VOLT_PID_P					0.2f					/**< Proportional part of the voltage PID regulator */
#define PS_VOLT_PID_I					3.0f					/**< Integral part of the voltage PID regulator */
#define PS_VOLT_PID_D					0						/**< Differential part of the voltage PID regulator */

#define DDS_MIN_FREQ					0.001f					/**< Minimum allowed DDS frequency setting */
#define DDS_MAX_FREQ					20000  					/**< Maximum allowed DDS frequency setting */
#define DDS_MIN_AMPLITUDE				0						/**< Minimum allowed DDS amplitude setting */
#define DDS_MAX_AMPLITUDE				20						/**< Maximum allowed DDS amplitude setting */
#define DDS_MIN_OFFSET					-10						/**< Minimum allowed DDS offset setting */
#define DDS_MAX_OFFSET					10						/**< Maximum allowed DDS offset setting */
#define DDS_USER_DEFINED_WAVEFORMS_ENABLED						/**< Comment this out to disable support for user defined waveforms for both DDS channels */

#endif /* CONFIGURATION_H_ */