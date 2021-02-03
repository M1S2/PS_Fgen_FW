/*
 * SCPI_Cmds_Output.cpp
 *
 * Created: 03.12.2020 18:16:17
 *  Author: V17
 */ 

#include "SCPI_Device.h"
#include "../Device.h"

scpi_result_t scpi_cmd_outputState(scpi_t * context)
{
	int32_t numbers[1];
	SCPI_CommandNumbers(context, numbers, 1, 1);
	
	scpi_bool_t state;
	if(!SCPI_ParamBool(context, &state, TRUE))
	{
		return SCPI_RES_ERR;
	}
	
	int32_t channelNum = numbers[0];
	if(channelNum < 0 || channelNum >= NUM_OUTPUT_CHANNELS)
	{
		const char* msg = "Channel number out of range.";
		SCPI_ResultCharacters(context, msg, strlen(msg));
		return SCPI_RES_ERR;
	}
	
	bool status = Device.Channels[channelNum].SetEnabled(state);
	
	if(!status)
	{
		const char* msg = "Channel doesn't support to change the output state.";
		SCPI_ResultCharacters(context, msg, strlen(msg));
		return SCPI_RES_ERR;
	}
	
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_outputStateQ(scpi_t * context)
{
	int32_t numbers[1];
	SCPI_CommandNumbers(context, numbers, 1, 1);
	
	int32_t channelNum = numbers[0];
	if(channelNum < 0 || channelNum >= NUM_OUTPUT_CHANNELS)
	{
		const char* msg = "Channel number out of range.";
		SCPI_ResultCharacters(context, msg, strlen(msg));
		return SCPI_RES_ERR;
	}
	
	SCPI_ResultBool(context, Device.Channels[channelNum].GetEnabled());
	
	return SCPI_RES_OK;
}