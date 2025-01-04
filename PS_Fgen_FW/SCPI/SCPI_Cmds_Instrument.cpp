/*
 * SCPI_Cmds_Instrument.cpp
 * Created: 09.02.2021 17:10:44
 * Author: Markus Scheich
 */ 

#include "SCPI_Device.h"
#include "../Device.h"
#include <stdio.h>

static scpi_choice_def_t channel_choice[] =
{
	{ "CH1", 1 },
	{ "CH2", 2 },
	{ "CH3", 3 },
	{ "CH4", 4 },
	{ "CH5", 5 }, 
	SCPI_CHOICE_LIST_END /* termination of option list */
};

scpi_result_t scpi_cmd_instrumentSelect(scpi_t * context)
{
	int32_t channelIdx;
	if (!SCPI_ParamChoice(context, channel_choice, &channelIdx, TRUE))
	{
		return SCPI_RES_ERR;
	}
	channelIdx--;	// channel index must be zero based

	if (channelIdx < 0 || channelIdx >= NUM_CHANNELS)
	{
		const char* msg = "Channel number out of range.";
		SCPI_ResultCharacters(context, msg, strlen(msg));
		return SCPI_RES_ERR;
	}
	
	Device.SelectedScpiChannelIndex = channelIdx;
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_instrumentSelectQ(scpi_t * context)
{  
	const char* buffer;
	if (!SCPI_ChoiceToName(channel_choice, (int32_t)(Device.SelectedScpiChannelIndex + 1), &buffer)) { return SCPI_RES_ERR; }

	SCPI_ResultCharacters(context, buffer, strlen(buffer));
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_instrumentNSelect(scpi_t * context)
{
	int32_t channelIdx;
	if (!SCPI_ParamInt32(context, &channelIdx, TRUE))
	{
		return SCPI_RES_ERR;
	}
	channelIdx--;	// channel index must be zero based

	if (channelIdx < 0 || channelIdx >= NUM_CHANNELS)
	{
		const char* msg = "Channel number out of range.";
		SCPI_ResultCharacters(context, msg, strlen(msg));
		return SCPI_RES_ERR;
	}

	Device.SelectedScpiChannelIndex = channelIdx;
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_instrumentNSelectQ(scpi_t * context)
{
	SCPI_ResultInt32(context, (Device.SelectedScpiChannelIndex + 1));
	return SCPI_RES_OK;
}