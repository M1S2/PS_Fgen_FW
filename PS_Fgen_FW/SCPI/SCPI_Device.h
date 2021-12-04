/**
 * @file	SCPI_Device.h
 * @date	01.12.2020 18:08:03
 * @author	Markus Scheich
 * @brief	Containing defines and function prototypes of the SCPI part of the device.
 */ 

#ifndef SCPI_DEVICE_H_
#define SCPI_DEVICE_H_

#include "../libraries/scpi-parser/inc/scpi/parser.h"
#include "../libraries/scpi-parser/inc/scpi/ieee488.h"
#include "../libraries/scpi-parser/inc/scpi/error.h"
#include "../libraries/scpi-parser/inc/scpi/constants.h"
#include "../libraries/scpi-parser/inc/scpi/minimal.h"
#include "../libraries/scpi-parser/inc/scpi/units.h"
#include "../libraries/scpi-parser/inc/scpi/utils.h"
#include "../libraries/scpi-parser/inc/scpi/expression.h"

#include "../Configuration.h"

/**
 * Available SCPI channel parameters.
 * Enumeration with all channel parameters that are available through the SCPI interface.
 */
typedef enum SCPIChannelParameters
{
	SCPI_CHPARAM_OUTPUTSTATE,				/**< Output state (Enabled/Disabled) channel parameter. */ 
	SCPI_CHPARAM_AMPLITUDE,					/**< Amplitude channel parameter. */
	SCPI_CHPARAM_OFFSET,					/**< Offset channel parameter. */
	SCPI_CHPARAM_CURRENT,					/**< Current channel parameter. */
	SCPI_CHPARAM_FREQUENCY,					/**< Frequency channel parameter. */
	SCPI_CHPARAM_SIGNALFORM,				/**< Signal form channel parameter. */
	#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
		SCPI_CHPARAM_USERWAVEFORMDATA,		/**< User defined waveform channel parameter. */ 
	#endif
	SCPI_CHPARAM_MEASURED_AMPLITUDE,		/**< Measured amplitude channel parameter. */
	SCPI_CHPARAM_MEASURED_CURRENT,			/**< Measured current channel parameter. */
	SCPI_CHPARAM_MEASURED_POWER,			/**< Measured power channel parameter. */
	SCPI_CHPARAM_OVP_LEVEL,					/**< Over voltage protection level channel parameter. */
	SCPI_CHPARAM_OVP_STATE,					/**< Over voltage protection state (Enabled/Disabled) channel parameter. */
	SCPI_CHPARAM_OVP_DELAY,					/**< Over voltage protection delay channel parameter. */
	SCPI_CHPARAM_OCP_LEVEL,					/**< Over current protection level channel parameter. */
	SCPI_CHPARAM_OCP_STATE,					/**< Over current protection state (Enabled/Disabled) channel parameter. */
	SCPI_CHPARAM_OCP_DELAY,					/**< Over current protection delay channel parameter. */
	SCPI_CHPARAM_OPP_LEVEL,					/**< Over power protection level channel parameter. */
	SCPI_CHPARAM_OPP_STATE,					/**< Over power protection state (Enabled/Disabled) channel parameter. */
	SCPI_CHPARAM_OPP_DELAY,					/**< Over power protection delay channel parameter. */
} SCPIChannelParameters_t;

//----------------------------------------------------------------------------------------------------------

extern scpi_t scpi_context;

/**
 * Write data to SCPI output.
 * The SCPI parser uses this function to write the given data to an output stream (e.g. for user response).
 * @param context SCPI parser context
 * @param data Data to write
 * @param len Length of the data in bytes
 * @return Number of written bytes
 */
size_t SCPI_Write(scpi_t * context, const char * data, size_t len);

/**
 * Flush data to SCPI output.
 * @param context SCPI parser context
 * @return Flush status (SCPI_RES_OK or SCPI_RES_ERR)
 */
scpi_result_t SCPI_Flush(scpi_t * context);

/**
 * Emit error to the SCPI output.
 * @param context SCPI parser context
 * @param err Error to emit
 */
int SCPI_Error(scpi_t * context, int_fast16_t err);

/**
 * Wrapper function to control interface from context
 * @param context SCPI parser context
 * @param ctrl Number of control message
 * @param value Value of related register
 */
scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val);

/**
 * Called when a *RST command is received
 * @param context SCPI parser context
 * @return Reset status (SCPI_RES_OK or SCPI_RES_ERR)
 */
scpi_result_t SCPI_Reset(scpi_t * context);

/** Initialize the SCPI part of the device. */
void SCPI_Init_Device();

/**
 * Return a "Channel number out of range." message with the SCPI context.
 * @param context SCPI parser context
 * @return Always returns SCPI_RES_ERR
 */
scpi_result_t SCPI_SetResult_ChannelOutOfRange(scpi_t * context);

/**
 * Return a "Operation not supported by channel." message with the SCPI context.
 * @param context SCPI parser context
 * @return Always returns SCPI_RES_ERR
 */
scpi_result_t SCPI_SetResult_NotSupportedByChannel(scpi_t * context);

/**
 * Query the value of a specific channel parameter and write it to the output.
 * @param context SCPI parser context
 * @param paramType Which channel parameter should be returned
 * @return Query status (SCPI_RES_OK or SCPI_RES_ERR)
 */
scpi_result_t SCPI_QueryChannelParameter(scpi_t * context, SCPIChannelParameters_t paramType);

/**
 * Set the value of a specific channel parameter to the value given with the SCPI parameter.
 * @param context SCPI parser context
 * @param paramType Which channel parameter should be set
 * @return Set status (SCPI_RES_OK or SCPI_RES_ERR)
 */
scpi_result_t SCPI_SetChannelParameter(scpi_t * context, SCPIChannelParameters_t paramType);

/**
 * Get the numeric repesentation from the SCPI parameter.
 * Special parameters like MIN, DEF, UP, ... are supported. The value is also coerced to be between min and max.
 * @param context SCPI parser context
 * @param param scpi_number_t returned by the SCPI_ParamNumber() method
 * @param value Output variable with the numeric representation of the SCPI parameter
 * @param unit SCPI base unit that is used to discard values with other units.
 * @param min Minimum value. All values below this number are coerced to min.
 * @param max Maximum value. All values above this number are coerced to max.
 * @param def Default value. This value is used if DEF is received as SCPI parameter.
 * @param step Step value. This value is used to step up or down the value if UP or DOWN are received as SCPI parameters.
 * @return False on error, otherwise true.
 */
bool SCPI_GetNumericFromParam(scpi_t* context, const scpi_number_t& param, float& value, scpi_unit_t unit, float min, float max, float def, float step);

/******* SCPI_Cmds_Output.cpp ***************************/
scpi_result_t scpi_cmd_outputState(scpi_t * context);								/**< Function executed when the OUTPut#[:STATe] command was received. */
scpi_result_t scpi_cmd_outputStateQ(scpi_t * context);								/**< Function executed when the OUTPut#[:STATe]? command was received. */
scpi_result_t scpi_cmd_outputGeneral(scpi_t * context);								/**< Function executed when the OUTPut:GENeral command was received. */
scpi_result_t scpi_cmd_outputGeneralQ(scpi_t * context);							/**< Function executed when the OUTPut:GENeral? command was received. */

/******* SCPI_Cmds_Measure.cpp ***************************/
scpi_result_t scpi_cmd_measureScalarCurrentDCQ(scpi_t * context);					/**< Function executed when the MEASure#[:SCALar]:CURRent[:DC]? command was received. */
scpi_result_t scpi_cmd_measureScalarPowerDCQ(scpi_t * context);						/**< Function executed when the MEASure#[:SCALar]:POWer[:DC]? command was received. */
scpi_result_t scpi_cmd_measureScalarVoltageDCQ(scpi_t * context);					/**< Function executed when the MEASure#[:SCALar][:VOLTage][:DC]? command was received. */

/******* SCPI_Cmds_Source.cpp ****************************/
scpi_result_t scpi_cmd_sourceVoltageLevelImmediateAmplitude(scpi_t * context);		/**< Function executed when the SOURce#:VOLTage[:LEVel][:IMMediate][:AMPLitude] command was received. */
scpi_result_t scpi_cmd_sourceVoltageLevelImmediateAmplitudeQ(scpi_t * context);		/**< Function executed when the SOURce#:VOLTage[:LEVel][:IMMediate][:AMPLitude]? command was received. */
scpi_result_t scpi_cmd_sourceVoltageLevelImmediateOffset(scpi_t * context);			/**< Function executed when the SOURce#:VOLTage[:LEVel][:IMMediate]:OFFSet command was received. */
scpi_result_t scpi_cmd_sourceVoltageLevelImmediateOffsetQ(scpi_t * context);		/**< Function executed when the SOURce#:VOLTage[:LEVel][:IMMediate]:OFFSet? command was received. */
scpi_result_t scpi_cmd_sourceVoltageProtectionLevel(scpi_t * context);				/**< Function executed when the SOURce#:VOLTage:PROTection[:LEVel] command was received. */
scpi_result_t scpi_cmd_sourceVoltageProtectionLevelQ(scpi_t * context);				/**< Function executed when the SOURce#:VOLTage:PROTection[:LEVel]? command was received. */
scpi_result_t scpi_cmd_sourceVoltageProtectionState(scpi_t * context);				/**< Function executed when the SOURce#:VOLTage:PROTection:STATe command was received. */
scpi_result_t scpi_cmd_sourceVoltageProtectionStateQ(scpi_t * context);				/**< Function executed when the SOURce#:VOLTage:PROTection:STATe? command was received. */
scpi_result_t scpi_cmd_sourceVoltageProtectionDelay(scpi_t * context);				/**< Function executed when the SOURce#:VOLTage:PROTection:DELay command was received. */
scpi_result_t scpi_cmd_sourceVoltageProtectionDelayQ(scpi_t * context);				/**< Function executed when the SOURce#:VOLTage:PROTection:DELay? command was received. */
scpi_result_t scpi_cmd_sourceVoltageProtectionTrippedQ(scpi_t * context);			/**< Function executed when the SOURce#:VOLTage:PROTection:TRIPped? command was received. */
scpi_result_t scpi_cmd_sourceVoltageProtectionClear(scpi_t * context);				/**< Function executed when the SOURce#:VOLTage:PROTection:CLEar command was received. */
scpi_result_t scpi_cmd_sourceCurrentLevelImmediateAmplitude(scpi_t * context);		/**< Function executed when the SOURce#:CURRent[:LEVel][:IMMediate][:AMPLitude] command was received. */
scpi_result_t scpi_cmd_sourceCurrentLevelImmediateAmplitudeQ(scpi_t * context);		/**< Function executed when the SOURce#:CURRent[:LEVel][:IMMediate][:AMPLitude]? command was received. */
scpi_result_t scpi_cmd_sourceCurrentProtectionLevel(scpi_t * context);				/**< Function executed when the SOURce#:CURRent:PROTection[:LEVel] command was received. */
scpi_result_t scpi_cmd_sourceCurrentProtectionLevelQ(scpi_t * context);				/**< Function executed when the SOURce#:CURRent:PROTection[:LEVel]? command was received. */
scpi_result_t scpi_cmd_sourceCurrentProtectionState(scpi_t * context);				/**< Function executed when the SOURce#:CURRent:PROTection:STATe command was received. */
scpi_result_t scpi_cmd_sourceCurrentProtectionStateQ(scpi_t * context);				/**< Function executed when the SOURce#:CURRent:PROTection:STATe? command was received. */
scpi_result_t scpi_cmd_sourceCurrentProtectionDelay(scpi_t * context);				/**< Function executed when the SOURce#:CURRent:PROTection:DELay command was received. */
scpi_result_t scpi_cmd_sourceCurrentProtectionDelayQ(scpi_t * context);				/**< Function executed when the SOURce#:CURRent:PROTection:DELay? command was received. */
scpi_result_t scpi_cmd_sourceCurrentProtectionTrippedQ(scpi_t * context);			/**< Function executed when the SOURce#:CURRent:PROTection:TRIPped? command was received. */
scpi_result_t scpi_cmd_sourceCurrentProtectionClear(scpi_t * context);				/**< Function executed when the SOURce#:CURRent:PROTection:CLEar command was received. */
scpi_result_t scpi_cmd_sourcePowerProtectionLevel(scpi_t * context);				/**< Function executed when the SOURce#:POWer:PROTection[:LEVel] command was received. */
scpi_result_t scpi_cmd_sourcePowerProtectionLevelQ(scpi_t * context);				/**< Function executed when the SOURce#:POWer:PROTection[:LEVel]? command was received. */
scpi_result_t scpi_cmd_sourcePowerProtectionState(scpi_t * context);				/**< Function executed when the SOURce#:POWer:PROTection:STATe command was received. */
scpi_result_t scpi_cmd_sourcePowerProtectionStateQ(scpi_t * context);				/**< Function executed when the SOURce#:POWer:PROTection:STATe? command was received. */
scpi_result_t scpi_cmd_sourcePowerProtectionDelay(scpi_t * context);				/**< Function executed when the SOURce#:POWer:PROTection:DELay command was received. */
scpi_result_t scpi_cmd_sourcePowerProtectionDelayQ(scpi_t * context);				/**< Function executed when the SOURce#:POWer:PROTection:DELay? command was received. */
scpi_result_t scpi_cmd_sourcePowerProtectionTrippedQ(scpi_t * context);				/**< Function executed when the SOURce#:POWer:PROTection:TRIPped? command was received. */
scpi_result_t scpi_cmd_sourcePowerProtectionClear(scpi_t * context);				/**< Function executed when the SOURce#:POWer:PROTection:CLEar command was received. */
scpi_result_t scpi_cmd_sourceFrequencyFixed(scpi_t * context);						/**< Function executed when the SOURce#:FREQuency[:CW] command was received. */
scpi_result_t scpi_cmd_sourceFrequencyFixedQ(scpi_t * context);						/**< Function executed when the SOURce#:FREQuency[:CW]? command was received. */
scpi_result_t scpi_cmd_sourceFunctionShape(scpi_t * context);						/**< Function executed when the SOURce#:FUNCtion[:SHAPe] command was received. */
scpi_result_t scpi_cmd_sourceFunctionShapeQ(scpi_t * context);						/**< Function executed when the SOURce#:FUNCtion[:SHAPe]? command was received. */
scpi_result_t scpi_cmd_sourceFunctionModeQ(scpi_t * context);						/**< Function executed when the SOURce#:FUNCtion:MODE? command was received. */
#if defined DDS_USER_DEFINED_WAVEFORMS_ENABLED && defined DDS_SUBSYSTEM_ENABLED
	scpi_result_t scpi_cmd_sourceFunctionData(scpi_t * context);					/**< Function executed when the SOURce#:FUNCtion:DATa command was received. */
#endif

/******* SCPI_Cmds_System.cpp ****************************/
scpi_result_t scpi_cmd_systemCapabilityQ(scpi_t * context);							/**< Function executed when the SYSTem:CAPability? command was received. */
scpi_result_t scpi_cmd_systemLocal(scpi_t * context);								/**< Function executed when the SYSTem:LOCal command was received. */
scpi_result_t scpi_cmd_systemRemote(scpi_t * context);								/**< Function executed when the SYSTem:REMote command was received. */
scpi_result_t scpi_cmd_systemRWLock(scpi_t * context);								/**< Function executed when the SYSTem:RWLock command was received. */
scpi_result_t scpi_cmd_systemChannelCountQ(scpi_t * context);						/**< Function executed when the SYSTem:CHANnel:COUNt? command was received. */
scpi_result_t scpi_cmd_systemCPUFirmwareQ(scpi_t * context);						/**< Function executed when the SYSTem:CPU:FIRMware? command was received. */
scpi_result_t scpi_cmd_systemCPUInformationTypeQ(scpi_t * context);					/**< Function executed when the SYSTem:CPU:INFOrmation:TYPE? command was received. */
scpi_result_t scpi_cmd_systemCPUSnoQ(scpi_t * context);								/**< Function executed when the SYSTem:CPU:SNO? command was received. */
scpi_result_t scpi_cmd_systemCPUVersionQ(scpi_t * context);							/**< Function executed when the SYSTem:CPU:VERSion? command was received. */
scpi_result_t scpi_cmd_systemCommunicateSerialBaud(scpi_t * context);				/**< Function executed when the SYSTem:COMMunicate:SERial:BAUD command was received. */
scpi_result_t scpi_cmd_systemCommunicateSerialBaudQ(scpi_t * context);				/**< Function executed when the SYSTem:COMMunicate:SERial:BAUD? command was received. */
scpi_result_t scpi_cmd_systemCommunicateSerialEcho(scpi_t * context);				/**< Function executed when the SYSTem:COMMunicate:SERial:ECHO command was received. */
scpi_result_t scpi_cmd_systemCommunicateSerialEchoQ(scpi_t * context);				/**< Function executed when the SYSTem:COMMunicate:SERial:ECHO? command was received. */
scpi_result_t scpi_cmd_systemMeasureScalarVoltageDCQ(scpi_t * context);				/**< Function executed when the SYSTem:MEASure[:SCALar][:VOLTage][:DC]? command was received. */
scpi_result_t scpi_cmd_systemPonOutputDisable(scpi_t * context);					/**< Function executed when the SYSTem:PON:OUTPut:DISable command was received. */
scpi_result_t scpi_cmd_systemPonOutputDisableQ(scpi_t * context);					/**< Function executed when the SYSTem:PON:OUTPut:DISable? command was received. */

/******* SCPI_Cmds_Memory.cpp ****************************/
scpi_result_t scpi_cmd_memoryRcl(scpi_t * context);									/**< Function executed when the *RCL command was received. */
scpi_result_t scpi_cmd_memorySav(scpi_t * context);									/**< Function executed when the *SAV command was received. */
scpi_result_t scpi_cmd_memoryNStatesQ(scpi_t * context);							/**< Function executed when the MEMory:NSTates? command was received. */

/******* SCPI_Cmds_Instrument.cpp *************************/
scpi_result_t scpi_cmd_instrumentSelect(scpi_t * context);							/**< Function executed when the INSTrument[:SELect] command was received. */
scpi_result_t scpi_cmd_instrumentSelectQ(scpi_t * context);							/**< Function executed when the INSTrument[:SELect]? command was received. */
scpi_result_t scpi_cmd_instrumentNSelect(scpi_t * context);							/**< Function executed when the INSTrument:NSELect command was received. */
scpi_result_t scpi_cmd_instrumentNSelectQ(scpi_t * context);						/**< Function executed when the INSTrument:NSELect? command was received. */
scpi_result_t scpi_cmd_instrumentCatalogQ(scpi_t * context);						/**< Function executed when the INSTrument:CATalog? command was received. */
scpi_result_t scpi_cmd_instrumentCatalogFullQ(scpi_t * context);					/**< Function executed when the INSTrument:CATalog:FULL? command was received. */

#endif /* SCPI_DEVICE_H_ */