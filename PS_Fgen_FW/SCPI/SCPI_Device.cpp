/*
 * SCPI_Device.cpp
 * Created: 01.12.2020 18:08:13
 * Author: Markus Scheich
 */ 

#include "SCPI_Device.h"
#include "../Device.h"
#include <stdio.h>

scpi_t scpi_context;
char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];
char resultBuffer[50];

#ifdef DDS_SUBSYSTEM_ENABLED
	static scpi_choice_def_t signalform_choice[] =
	{
		{ "SINusoid", SIGNALFORM_SINE },
		{ "SQUare", SIGNALFORM_RECTANGLE },
		{ "TRIangle", SIGNALFORM_TRIANGLE },
		{ "SAWtooth", SIGNALFORM_SAWTOOTH },
		{ "DC", SIGNALFORM_DC },
		#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
			{ "USER", SIGNALFORM_USER_SIGNAL },
		#endif
		SCPI_CHOICE_LIST_END /* termination of option list */
	};
#endif

const scpi_command_t scpi_commands[] = 
{
	/* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
	{"*CLS", SCPI_CoreCls, 0},
	{"*ESE", SCPI_CoreEse, 0},
	{"*ESE?", SCPI_CoreEseQ, 0},
	{"*ESR?", SCPI_CoreEsrQ, 0},
	{"*IDN?", SCPI_CoreIdnQ, 0},
	{"*OPC", SCPI_CoreOpc, 0},
	{"*OPC?", SCPI_CoreOpcQ, 0},
	{"*RCL", scpi_cmd_memoryRcl, 0},
	{"*RST", SCPI_CoreRst, 0},
	{"*SAV", scpi_cmd_memorySav, 0},
	{"*SRE", SCPI_CoreSre, 0},
	{"*SRE?", SCPI_CoreSreQ, 0},
	{"*STB?", SCPI_CoreStbQ, 0},
	//{"*TST?", My_CoreTstQ, 0},
	{"*WAI", SCPI_CoreWai, 0},
		
	/* Required SCPI commands (SCPI std V1999.0 4.2.1) */
	{"SYSTem:ERRor[:NEXT]?", SCPI_SystemErrorNextQ, 0},
	{"SYSTem:ERRor:COUNt?", SCPI_SystemErrorCountQ, 0},
	{"SYSTem:VERSion?", SCPI_SystemVersionQ, 0},

	/****** Output Subsystem ***************************/
	{"OUTPut#[:STATe]", scpi_cmd_outputState, 0},
	{"OUTPut#[:STATe]?", scpi_cmd_outputStateQ, 0},
	{"OUTPut:GENeral", scpi_cmd_outputGeneral, 0},
	{"OUTPut:GENeral?", scpi_cmd_outputGeneralQ, 0},
			
	/****** Measure Subsystem **************************/
	{"MEASure#[:SCALar]:CURRent[:DC]?", scpi_cmd_measureScalarCurrentDCQ, 0},
	{"MEASure#[:SCALar]:POWer[:DC]?", scpi_cmd_measureScalarPowerDCQ, 0},
	{"MEASure#[:SCALar][:VOLTage][:DC]?", scpi_cmd_measureScalarVoltageDCQ, 0},
						
	/****** Source Subsystem ***************************/
	{"SOURce#:VOLTage[:LEVel][:IMMediate][:AMPLitude]", scpi_cmd_sourceVoltageLevelImmediateAmplitude, 0},
	{"SOURce#:VOLTage[:LEVel][:IMMediate][:AMPLitude]?", scpi_cmd_sourceVoltageLevelImmediateAmplitudeQ, 0},
	{"SOURce#:VOLTage[:LEVel][:IMMediate]:OFFSet", scpi_cmd_sourceVoltageLevelImmediateOffset, 0},
	{"SOURce#:VOLTage[:LEVel][:IMMediate]:OFFSet?", scpi_cmd_sourceVoltageLevelImmediateOffsetQ, 0},
	{"SOURce#:VOLTage:PROTection[:LEVel]", scpi_cmd_sourceVoltageProtectionLevel, 0},
	{"SOURce#:VOLTage:PROTection[:LEVel]?", scpi_cmd_sourceVoltageProtectionLevelQ, 0},
	{"SOURce#:VOLTage:PROTection:STATe", scpi_cmd_sourceVoltageProtectionState, 0},
	{"SOURce#:VOLTage:PROTection:STATe?", scpi_cmd_sourceVoltageProtectionStateQ, 0},
	{"SOURce#:VOLTage:PROTection:DELay", scpi_cmd_sourceVoltageProtectionDelay, 0},
	{"SOURce#:VOLTage:PROTection:DELay?", scpi_cmd_sourceVoltageProtectionDelayQ, 0},
	{"SOURce#:VOLTage:PROTection:TRIPped?", scpi_cmd_sourceVoltageProtectionTrippedQ, 0},
	{"SOURce#:VOLTage:PROTection:CLEar", scpi_cmd_sourceVoltageProtectionClear, 0},
	{"SOURce#:CURRent:PROTection[:LEVel]", scpi_cmd_sourceCurrentProtectionLevel, 0},
	{"SOURce#:CURRent:PROTection[:LEVel]?", scpi_cmd_sourceCurrentProtectionLevelQ, 0},
	{"SOURce#:CURRent:PROTection:STATe", scpi_cmd_sourceCurrentProtectionState, 0},
	{"SOURce#:CURRent:PROTection:STATe?", scpi_cmd_sourceCurrentProtectionStateQ, 0},
	{"SOURce#:CURRent:PROTection:DELay", scpi_cmd_sourceCurrentProtectionDelay, 0},
	{"SOURce#:CURRent:PROTection:DELay?", scpi_cmd_sourceCurrentProtectionDelayQ, 0},
	{"SOURce#:CURRent:PROTection:TRIPped?", scpi_cmd_sourceCurrentProtectionTrippedQ, 0},
	{"SOURce#:CURRent:PROTection:CLEar", scpi_cmd_sourceCurrentProtectionClear, 0},
	{"SOURce#:POWer:PROTection[:LEVel]", scpi_cmd_sourcePowerProtectionLevel, 0},
	{"SOURce#:POWer:PROTection[:LEVel]?", scpi_cmd_sourcePowerProtectionLevelQ, 0},
	{"SOURce#:POWer:PROTection:STATe", scpi_cmd_sourcePowerProtectionState, 0},
	{"SOURce#:POWer:PROTection:STATe?", scpi_cmd_sourcePowerProtectionStateQ, 0},
	{"SOURce#:POWer:PROTection:DELay", scpi_cmd_sourcePowerProtectionDelay, 0},
	{"SOURce#:POWer:PROTection:DELay?", scpi_cmd_sourcePowerProtectionDelayQ, 0},
	{"SOURce#:POWer:PROTection:TRIPped?", scpi_cmd_sourcePowerProtectionTrippedQ, 0},
	{"SOURce#:POWer:PROTection:CLEar", scpi_cmd_sourcePowerProtectionClear, 0},			
	{"SOURce#:FREQuency[:CW]", scpi_cmd_sourceFrequencyFixed, 0},
	{"SOURce#:FREQuency[:CW]?", scpi_cmd_sourceFrequencyFixedQ, 0},	
	{"SOURce#:FUNCtion[:SHAPe]", scpi_cmd_sourceFunctionShape, 0},
	{"SOURce#:FUNCtion[:SHAPe]?", scpi_cmd_sourceFunctionShapeQ, 0},
	{"SOURce#:FUNCtion:MODE?", scpi_cmd_sourceFunctionModeQ, 0},
	#if defined DDS_USER_DEFINED_WAVEFORMS_ENABLED && defined DDS_SUBSYSTEM_ENABLED
		{"SOURce#:FUNCtion:DATa", scpi_cmd_sourceFunctionData, 0},
	#endif
	
	/****** System Subsystem ***************************/
	{"SYSTem:LOCal", scpi_cmd_systemLocal, 0},
	{"SYSTem:REMote", scpi_cmd_systemRemote, 0},
	{"SYSTem:RWLock", scpi_cmd_systemRWLock, 0},
	{"SYSTem:CHANnel:COUNt?", scpi_cmd_systemChannelCountQ, 0},
	{"SYSTem:FIRMware?", scpi_cmd_systemFirmwareQ, 0},
	{"SYSTem:SNO?", scpi_cmd_systemSnoQ, 0},
	{"SYSTem:MEASure[:SCALar][:VOLTage][:DC]?", scpi_cmd_systemMeasureScalarVoltageDCQ, 0},

	/****** Instrument Subsystem ***********************/
	{"INSTrument[:SELect]", scpi_cmd_instrumentSelect, 0},
	{"INSTrument[:SELect]?", scpi_cmd_instrumentSelectQ, 0},
	{"INSTrument:NSELect", scpi_cmd_instrumentNSelect, 0},
	{"INSTrument:NSELect?", scpi_cmd_instrumentNSelectQ, 0},
									
	SCPI_CMD_LIST_END
};

scpi_interface_t scpi_interface = 
{
	/*.error = */ SCPI_Error,
	/*.write = */ SCPI_Write,
	/*.control = */ SCPI_Control,
	/*.flush = */ SCPI_Flush,
	/*.reset = */ SCPI_Reset,
};

size_t SCPI_Write(scpi_t * context, const char * data, size_t len) 
{
	(void) context;
	Serial.write(data);
	return len;				/* OK */
	//return fwrite(data, 1, len, stdout);
}

scpi_result_t SCPI_Flush(scpi_t * context) 
{
	(void) context;
	return SCPI_RES_OK;
}

int SCPI_Error(scpi_t * context, int_fast16_t err) 
{
	(void) context;
	
	sprintf(resultBuffer, "**ERROR: %d, \"%s\"\r\n", (int16_t) err, SCPI_ErrorTranslate(err));
	Serial.write(resultBuffer);
	return 0;
}

scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val) 
{
	(void) context;

	if (SCPI_CTRL_SRQ == ctrl) 
	{
		sprintf(resultBuffer, "**SRQ: 0x%X (%d)\r\n", val, val);
	} 
	else
	{
		sprintf(resultBuffer, "**CTRL %02x: 0x%X (%d)\r\n", ctrl, val, val);
	}
	Serial.write(resultBuffer);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(scpi_t * context) 
{
	(void) context;
	Device.ResetDevice();
	return SCPI_RES_OK;
}

void SCPI_Init_Device()
{
	SCPI_Init(&scpi_context,
				scpi_commands,
				&scpi_interface,
				scpi_units_def,
				SCPI_IDN_MANUFACTURER, SCPI_IDN_MODEL, SCPI_IDN_SERIAL_NUMBER, SCPI_IDN_SOFTWARE_REVISION,
				scpi_input_buffer, SCPI_INPUT_BUFFER_LENGTH,
				scpi_error_queue_data, SCPI_ERROR_QUEUE_SIZE);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SetResult_ChannelOutOfRange(scpi_t * context)
{
	const char* msg = "Channel number out of range.";
	SCPI_ResultCharacters(context, msg, strlen(msg));
	return SCPI_RES_ERR;
}

scpi_result_t SCPI_SetResult_NotSupportedByChannel(scpi_t * context)
{
	const char* msg = "Operation not supported by channel.";
	SCPI_ResultCharacters(context, msg, strlen(msg));
	return SCPI_RES_ERR;
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_QueryChannelParameter(scpi_t * context, SCPIChannelParameters_t paramType)
{
	int32_t sourceNumbers[1];
	SCPI_CommandNumbers(context, sourceNumbers, 1, Device.SelectedScpiChannelIndex);
	
	int32_t channelNum = sourceNumbers[0];
	if(channelNum < 0 || channelNum >= NUM_CHANNELS)
	{
		return SCPI_SetResult_ChannelOutOfRange(context);
	}
	
	#ifdef PS_SUBSYSTEM_ENABLED
		if (Device.Channels[channelNum]->GetChannelType() == POWER_SUPPLY_CHANNEL_TYPE)
		{
			PS_Channel* psChannel = (PS_Channel*)Device.Channels[channelNum];
			switch(paramType)
			{
				case SCPI_CHPARAM_OUTPUTSTATE: SCPI_ResultBool(context, psChannel->GetEnabled()); break;
				case SCPI_CHPARAM_AMPLITUDE: SCPI_ResultFloat(context, psChannel->GetVoltage()); break;
				case SCPI_CHPARAM_MEASURED_AMPLITUDE: SCPI_ResultFloat(context, psChannel->MeasuredVoltage); break;
				case SCPI_CHPARAM_MEASURED_CURRENT: SCPI_ResultFloat(context, psChannel->MeasuredCurrent); break;
				case SCPI_CHPARAM_MEASURED_POWER: SCPI_ResultFloat(context, psChannel->MeasuredPower); break;
				case SCPI_CHPARAM_OVP_LEVEL: SCPI_ResultUInt8(context, psChannel->GetOvpLevel()); break;
				case SCPI_CHPARAM_OVP_STATE: SCPI_ResultBool(context, psChannel->GetOvpState()); break;
				case SCPI_CHPARAM_OVP_DELAY: SCPI_ResultFloat(context, psChannel->GetOvpDelay()); break;
				case SCPI_CHPARAM_OCP_LEVEL: SCPI_ResultFloat(context, psChannel->GetOcpLevel()); break;
				case SCPI_CHPARAM_OCP_STATE: SCPI_ResultBool(context, psChannel->GetOcpState()); break;
				case SCPI_CHPARAM_OCP_DELAY: SCPI_ResultFloat(context, psChannel->GetOcpDelay()); break;
				case SCPI_CHPARAM_OPP_LEVEL: SCPI_ResultFloat(context, psChannel->GetOppLevel()); break;
				case SCPI_CHPARAM_OPP_STATE: SCPI_ResultBool(context, psChannel->GetOppState()); break;
				case SCPI_CHPARAM_OPP_DELAY: SCPI_ResultFloat(context, psChannel->GetOppDelay()); break;
				default: return SCPI_SetResult_NotSupportedByChannel(context);
			}
			return SCPI_RES_OK;
		}
	#endif
	#ifdef DDS_SUBSYSTEM_ENABLED
		if (Device.Channels[channelNum]->GetChannelType() == DDS_CHANNEL_TYPE)
		{
			DDS_Channel* ddsChannel = (DDS_Channel*)Device.Channels[channelNum];
			switch(paramType)
			{
				case SCPI_CHPARAM_OUTPUTSTATE: SCPI_ResultBool(context, ddsChannel->GetEnabled()); break;
				case SCPI_CHPARAM_AMPLITUDE: SCPI_ResultFloat(context, ddsChannel->GetAmplitude()); break;
				case SCPI_CHPARAM_OFFSET: SCPI_ResultFloat(context, ddsChannel->GetOffset()); break;
				case SCPI_CHPARAM_FREQUENCY: SCPI_ResultFloat(context, ddsChannel->GetFrequency()); break;
				case SCPI_CHPARAM_SIGNALFORM: 
				{
					const char* buffer;
					if (!SCPI_ChoiceToName(signalform_choice, (int32_t)ddsChannel->GetSignalForm(), &buffer)) { return SCPI_RES_ERR; }
				
					SCPI_ResultCharacters(context, buffer, strlen(buffer));
					break;
				}
				default: return SCPI_SetResult_NotSupportedByChannel(context);
			}
			return SCPI_RES_OK;
		}
	#endif
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		if (Device.Channels[channelNum]->GetChannelType() == DMM_CHANNEL_TYPE)
		{
			DMM_Channel* dmmChannel = (DMM_Channel*)Device.Channels[channelNum];
			switch(paramType)
			{
				case SCPI_CHPARAM_MEASURED_AMPLITUDE: SCPI_ResultFloat(context, dmmChannel->MeasuredVoltage); break;
				default: return SCPI_SetResult_NotSupportedByChannel(context);
			}
			return SCPI_RES_OK;
		}
	#endif

	return SCPI_SetResult_NotSupportedByChannel(context);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SetChannelParameter(scpi_t * context, SCPIChannelParameters_t paramType)
{
	int32_t sourceNumbers[1];
	SCPI_CommandNumbers(context, sourceNumbers, 1, Device.SelectedScpiChannelIndex);
	
	int32_t channelNum = sourceNumbers[0];
	if(channelNum < 0 || channelNum >= NUM_CHANNELS)
	{
		return SCPI_SetResult_ChannelOutOfRange(context);
	}
	
	#ifdef PS_SUBSYSTEM_ENABLED
		if (Device.Channels[channelNum]->GetChannelType() == POWER_SUPPLY_CHANNEL_TYPE)
		{
			PS_Channel* psChannel = (PS_Channel*)Device.Channels[channelNum];
		
			switch(paramType)
			{
				case SCPI_CHPARAM_OUTPUTSTATE:
				{
					scpi_bool_t state;
					if(!SCPI_ParamBool(context, &state, TRUE)) { return SCPI_RES_ERR; }
					
					psChannel->SetEnabled(state);
					break;
				}
				case SCPI_CHPARAM_AMPLITUDE:
				{
					scpi_number_t param;
					if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE)) { return SCPI_RES_ERR; }
				
					float amplitude = psChannel->GetVoltage();
					if (!SCPI_GetNumericFromParam(context, param, amplitude, SCPI_UNIT_VOLT, psChannel->Voltage.Min, psChannel->Voltage.Max, psChannel->Voltage.Def, psChannel->Voltage.Step))
					{
						return SCPI_RES_ERR;
					}
					psChannel->SetVoltage(amplitude);
					break;
				}
				case SCPI_CHPARAM_OVP_LEVEL:
				{
					scpi_number_t param;
					if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE)) { return SCPI_RES_ERR; }
				
					float ovpLevel = (float)psChannel->GetOvpLevel();
					if (!SCPI_GetNumericFromParam(context, param, ovpLevel, SCPI_UNIT_NONE, psChannel->OvpLevel.Min, psChannel->OvpLevel.Max, psChannel->OvpLevel.Def, psChannel->OvpLevel.Step))
					{
						return SCPI_RES_ERR;
					}
					psChannel->SetOvpLevel((uint8_t)ovpLevel);
					break;
				}
				case SCPI_CHPARAM_OVP_STATE:
				{
					scpi_bool_t state;
					if(!SCPI_ParamBool(context, &state, TRUE)) { return SCPI_RES_ERR; }
				
					psChannel->SetOvpState(state);
					break;
				}			
				case SCPI_CHPARAM_OVP_DELAY: 
				{
					scpi_number_t param;
					if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE)) { return SCPI_RES_ERR; }
				
					float ovpDelay = psChannel->GetOvpDelay();
					if (!SCPI_GetNumericFromParam(context, param, ovpDelay, SCPI_UNIT_SECOND, psChannel->OvpDelay.Min, psChannel->OvpDelay.Max, psChannel->OvpDelay.Def, psChannel->OvpDelay.Step))
					{
						return SCPI_RES_ERR;
					}
					psChannel->SetOvpDelay(ovpDelay);
					break;
				}
				case SCPI_CHPARAM_OCP_LEVEL:
				{
					scpi_number_t param;
					if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE)) { return SCPI_RES_ERR; }
				
					float ocpLevel = (float)psChannel->GetOcpLevel();
					if (!SCPI_GetNumericFromParam(context, param, ocpLevel, SCPI_UNIT_NONE, psChannel->OcpLevel.Min, psChannel->OcpLevel.Max, psChannel->OcpLevel.Def, psChannel->OcpLevel.Step))
					{
						return SCPI_RES_ERR;
					}
					psChannel->SetOcpLevel(ocpLevel);
					break;
				}
				case SCPI_CHPARAM_OCP_STATE:
				{
					scpi_bool_t state;
					if(!SCPI_ParamBool(context, &state, TRUE)) { return SCPI_RES_ERR; }
				
					psChannel->SetOcpState(state);
					break;
				}
				case SCPI_CHPARAM_OCP_DELAY:
				{
					scpi_number_t param;
					if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE)) { return SCPI_RES_ERR; }
				
					float ocpDelay = psChannel->GetOcpDelay();
					if (!SCPI_GetNumericFromParam(context, param, ocpDelay, SCPI_UNIT_SECOND, psChannel->OcpDelay.Min, psChannel->OcpDelay.Max, psChannel->OcpDelay.Def, psChannel->OcpDelay.Step))
					{
						return SCPI_RES_ERR;
					}
					psChannel->SetOcpDelay(ocpDelay);
					break;
				}		
				case SCPI_CHPARAM_OPP_LEVEL:
				{
					scpi_number_t param;
					if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE)) { return SCPI_RES_ERR; }
				
					float oppLevel = psChannel->GetOppLevel();
					if (!SCPI_GetNumericFromParam(context, param, oppLevel, SCPI_UNIT_WATT, psChannel->OppLevel.Min, psChannel->OppLevel.Max, psChannel->OppLevel.Def, psChannel->OppLevel.Step))
					{
						return SCPI_RES_ERR;
					}
					psChannel->SetOppLevel(oppLevel);
					break;
				}
				case SCPI_CHPARAM_OPP_STATE:
				{
					scpi_bool_t state;
					if(!SCPI_ParamBool(context, &state, TRUE)) { return SCPI_RES_ERR; }
				
					psChannel->SetOppState(state);
					break;
				}
				case SCPI_CHPARAM_OPP_DELAY:
				{
					scpi_number_t param;
					if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE)) { return SCPI_RES_ERR; }
				
					float oppDelay = psChannel->GetOppDelay();
					if (!SCPI_GetNumericFromParam(context, param, oppDelay, SCPI_UNIT_SECOND, psChannel->OppDelay.Min, psChannel->OppDelay.Max, psChannel->OppDelay.Def, psChannel->OppDelay.Step))
					{
						return SCPI_RES_ERR;
					}
					psChannel->SetOppDelay(oppDelay);
					break;
				}				
				default: return SCPI_SetResult_NotSupportedByChannel(context);
			}
			return SCPI_RES_OK;
		}
	#endif
	#ifdef DDS_SUBSYSTEM_ENABLED
		if (Device.Channels[channelNum]->GetChannelType() == DDS_CHANNEL_TYPE)
		{
			DDS_Channel* ddsChannel = (DDS_Channel*)Device.Channels[channelNum];
		
			switch(paramType)
			{
				case SCPI_CHPARAM_OUTPUTSTATE:
				{
					scpi_bool_t state;
					if(!SCPI_ParamBool(context, &state, TRUE)) { return SCPI_RES_ERR; }

					ddsChannel->SetEnabled(state);
					break;
				}
				case SCPI_CHPARAM_AMPLITUDE:
				{
					scpi_number_t param;
					if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE)) { return SCPI_RES_ERR; }
					
					float amplitude = ddsChannel->GetAmplitude();
					if (!SCPI_GetNumericFromParam(context, param, amplitude, SCPI_UNIT_VOLT, ddsChannel->Amplitude.Min, ddsChannel->Amplitude.Max, ddsChannel->Amplitude.Def, ddsChannel->Amplitude.Step))
					{
						return SCPI_RES_ERR;
					}
					ddsChannel->SetAmplitude(amplitude);
					break;
				}
				case SCPI_CHPARAM_OFFSET:
				{
					scpi_number_t param;
					if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE)) { return SCPI_RES_ERR; }
					
					float offset = ddsChannel->GetOffset();
					if (!SCPI_GetNumericFromParam(context, param, offset, SCPI_UNIT_VOLT, ddsChannel->Offset.Min, ddsChannel->Offset.Max, ddsChannel->Offset.Def, ddsChannel->Offset.Step))
					{
						return SCPI_RES_ERR;
					}
					ddsChannel->SetOffset(offset);
					break;
				}
				case SCPI_CHPARAM_FREQUENCY:
				{
					scpi_number_t param;
					if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE)) { return SCPI_RES_ERR; }
					
					float frequency = ddsChannel->GetFrequency();
					if (!SCPI_GetNumericFromParam(context, param, frequency, SCPI_UNIT_HERTZ, ddsChannel->Frequency.Min, ddsChannel->Frequency.Max, ddsChannel->Frequency.Def, ddsChannel->Frequency.Step))
					{
						return SCPI_RES_ERR;
					}
					ddsChannel->SetFrequency(frequency);
					break;
				}
				case SCPI_CHPARAM_SIGNALFORM:
				{
					int32_t signalForm;
					if (!SCPI_ParamChoice(context, signalform_choice, &signalForm, TRUE)) { return SCPI_RES_ERR; }
				
					ddsChannel->SetSignalForm((SignalForms_t)signalForm);
					break;
				}
				#if defined DDS_USER_DEFINED_WAVEFORMS_ENABLED && defined DDS_SUBSYSTEM_ENABLED
				case SCPI_CHPARAM_USERWAVEFORMDATA:
				{
					/* SOURce[<channel>]:FUNCtion:DATa #<NumBytesDigits><NumBytes><Bytes>
					 * Split waveform in multiple packets (e.g. each 32 bytes with data) because the scpi_input_buffer can not hold the complete waveform at the same time.
					 * The packet length must be dividable by 2 because always 2 bytes are processed together (uint16_t).
					 * Each packet is appended to the DDS channel UserWaveTabe until the array is completely filled. Then the channel is updated.
					 * e.g. SOUR1:FUNC:DAT #23290ABCDEF90ABCDEF90ABCDEF90ABCDEF
					 *      SOUR1:FUNC:DAT #23212345678123456781234567812345678
					 *      ...
					 *
					 * Clear the receive index (fill the array from the beginning) by sending zero data:
					 * e.g. SOUR1:FUNC:DAT #10
					 */
							
					const char* userWaveformBuffer;
					size_t arbitraryBlockLen;
					if (!SCPI_ParamArbitraryBlock(context, &userWaveformBuffer, &arbitraryBlockLen, TRUE)) { return SCPI_RES_ERR; }
				
					// Set all array elements of the UserWaveTable to 0 if
					// - an command with zero data bytes was received or
					// - the ReceiveIndex is pointing to the first array element
					if(arbitraryBlockLen == 0 || ddsChannel->UserWaveTableReceiveIndex == 0)
					{
						for(int i = 0; i < (1 << DDS_QUANTIZER_BITS); i++)
						{
							ddsChannel->UserWaveTable[i] = 0;
						}
						ddsChannel->UserWaveTableReceiveIndex = 0;
					}
				
					// Combine 2 bytes (only lower 12 bits are used in DDS algorithm)
					for(unsigned int i = 0; i < arbitraryBlockLen; i+=2)
					{
						if((i + 1) >= arbitraryBlockLen || ddsChannel->UserWaveTableReceiveIndex >= (1 << DDS_QUANTIZER_BITS)) { break; }
						
						ddsChannel->UserWaveTable[ddsChannel->UserWaveTableReceiveIndex] = (uint16_t)((userWaveformBuffer[i] << 8) + userWaveformBuffer[i + 1]);
						ddsChannel->UserWaveTableReceiveIndex++;
					}
					
					// If the ReceiveIndex is larger than the number of array elements, the new waveform was fully received => Update the DDS channel and reset the ReceiveIndex for a new waveform
					if(ddsChannel->UserWaveTableReceiveIndex >= (1 << DDS_QUANTIZER_BITS))
					{
						ddsChannel->UserWaveTableReceiveIndex = 0;
						ddsChannel->UpdateWaveTable();
						Device.SaveSettingsDDSUserWaveforms();
						Serial.println("User Waveform updated");
					}
					
					break;
				}
				#endif
				default: return SCPI_SetResult_NotSupportedByChannel(context);
			}
			return SCPI_RES_OK;
		}
	#endif

	return SCPI_SetResult_NotSupportedByChannel(context);
}

//----------------------------------------------------------------------------------------------------------

bool SCPI_GetNumericFromParam(scpi_t* context, const scpi_number_t& param, float& value, scpi_unit_t unit, float min, float max, float def, float step) 
{
	if (param.special) 
	{
		if (param.content.tag == SCPI_NUM_MAX) 
		{
			value = max;
		}
		else if (param.content.tag == SCPI_NUM_MIN)
		{
			value = min;
		}
		else if (param.content.tag == SCPI_NUM_DEF) 
		{
			value = def;
		}
		else if (param.content.tag == SCPI_NUM_UP) 
		{
			value += step;
			if (value > max) value = max;
		}
		else if (param.content.tag == SCPI_NUM_DOWN) 
		{
			value -= step;
			if (value < min) value = min;
		}
		else 
		{
			SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
			return false;
		}
	}
	else 
	{
		if (param.unit != SCPI_UNIT_NONE && param.unit != unit) 
		{
			SCPI_ErrorPush(context, SCPI_ERROR_INVALID_SUFFIX);
			return false;
		}

		if ((float)param.content.value < min || (float)param.content.value > max)
		{
			SCPI_ErrorPush(context, SCPI_ERROR_DATA_OUT_OF_RANGE);
			return false;
		}
		value = (float)param.content.value;
	}

	return true;
}