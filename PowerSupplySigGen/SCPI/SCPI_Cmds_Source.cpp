/*
 * SCPI_Cmds_Source.cpp
 *
 * Created: 03.12.2020 18:35:51
 *  Author: V17
 */ 

#include "SCPI_Device.h"
#include "../Device.h"

scpi_result_t scpi_cmd_sourceVoltage(scpi_t * context)
{
	int32_t sourceNumbers[1];
	SCPI_CommandNumbers(context, sourceNumbers, 1, 1);
	
	scpi_number_t param;
	if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE))
	{
		return SCPI_RES_ERR;
	}
	
	int32_t channelNum = sourceNumbers[0];
	if(channelNum < 0 || channelNum >= NUM_OUTPUT_CHANNELS)
	{
		const char* msg = "Channel number out of range.";
		SCPI_ResultCharacters(context, msg, strlen(msg));
		return SCPI_RES_ERR;
	}
	
	if (Device.Channels[channelNum]->GetChannelType() == POWER_SUPPLY_CHANNEL_TYPE)
	{
		PS_Channel* psChannel = (PS_Channel*)Device.Channels[channelNum];
		
		float amplitude = psChannel->GetAmplitude();
		if (!SCPI_GetVoltageFromParam(context, param, amplitude, psChannel->Amplitude.Min, psChannel->Amplitude.Max, psChannel->Amplitude.Def, psChannel->Amplitude.Step))
		{
			return SCPI_RES_ERR;
		}
		psChannel->SetAmplitude(amplitude);
	}
	else
	{
		const char* msg = "Channel doesn't support to change the amplitude.";
		SCPI_ResultCharacters(context, msg, strlen(msg));
		return SCPI_RES_ERR;
	}
	
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_sourceVoltageQ(scpi_t * context)
{
	int32_t sourceNumbers[1];
	SCPI_CommandNumbers(context, sourceNumbers, 1, 1);
	
	int32_t channelNum = sourceNumbers[0];
	if(channelNum < 0 || channelNum >= NUM_OUTPUT_CHANNELS)
	{
		const char* msg = "Channel number out of range.";
		SCPI_ResultCharacters(context, msg, strlen(msg));
		return SCPI_RES_ERR;
	}
	
	if (Device.Channels[channelNum]->GetChannelType() == POWER_SUPPLY_CHANNEL_TYPE)
	{
		PS_Channel* psChannel = (PS_Channel*)Device.Channels[channelNum];
		SCPI_ResultFloat(context, psChannel->GetAmplitude());
	}
	else
	{
		const char* msg = "Channel doesn't support to read the amplitude.";
		SCPI_ResultCharacters(context, msg, strlen(msg));
		return SCPI_RES_ERR;
	}
	
	return SCPI_RES_OK;
}