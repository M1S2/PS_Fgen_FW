/*
 * SCPI_Cmds_Instrument.cpp
 *
 * Created: 09.02.2021 17:10:44
 *  Author: V17
 */ 

#include "SCPI_Device.h"
#include "../Device.h"
#include <stdio.h>

static scpi_choice_def_t channel_choice[] =
{
	{ "CH1", 1 },
	{ "CH2", 2 },
	{ "CH3", 3 },
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

	if (channelIdx < 0 || channelIdx >= NUM_OUTPUT_CHANNELS)
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
	char buffer[256] = { 0 };
	//sprintf(buffer, "CH%d", (Device.SelectedScpiChannelIndex + 1));
	
	int i=0;
	while(channel_choice[i].name != NULL && i < NUM_OUTPUT_CHANNELS)
	{
		if(channel_choice[i].tag == (Device.SelectedScpiChannelIndex + 1))
		{
			strcpy(buffer, channel_choice[i].name);
			break;
		}
		i++;
	}

	SCPI_ResultCharacters(context, buffer, strlen(buffer));
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_instrumentNSelect(scpi_t * context)
{
	int32_t channelIdx;
	if (!SCPI_ParamInt(context, &channelIdx, TRUE))
	{
		return SCPI_RES_ERR;
	}
	channelIdx--;	// channel index must be zero based

	if (channelIdx < 0 || channelIdx >= NUM_OUTPUT_CHANNELS)
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
	SCPI_ResultInt(context, (Device.SelectedScpiChannelIndex + 1));
	return SCPI_RES_OK;
}

//Returns a quoted string of the list of valid choices for the instrument channels (e.g. "CH1","CH2","CH3")
scpi_result_t scpi_cmd_instrumentCatalogQ(scpi_t * context)
{
	char buffer[256] = { 0 };
	int i=0;
	while(channel_choice[i].name != NULL && i < NUM_OUTPUT_CHANNELS)
	{
		sprintf(buffer + strlen(buffer), "\"%s\",", channel_choice[i].name);	//strlen() used to append to the buffer
		i++;
	}

	if(strlen(buffer) > 0) { buffer[strlen(buffer) - 1] = '\0'; }			// remove the last character (',') from the result string
	SCPI_ResultCharacters(context, buffer, strlen(buffer));
	return SCPI_RES_OK;
}

//Returns a list of string – number pairs (e.g. "CH1",1,"CH2",2,"CH3",3)
scpi_result_t scpi_cmd_instrumentCatalogFullQ(scpi_t * context)
{
	char buffer[256] = { 0 };
	int i=0;
	while(channel_choice[i].name != NULL && i < NUM_OUTPUT_CHANNELS)
	{
		sprintf(buffer + strlen(buffer), "\"%s\",%ld,", channel_choice[i].name, channel_choice[i].tag);	//strlen() used to append to the buffer
		i++;
	}

	if(strlen(buffer) > 0) { buffer[strlen(buffer) - 1] = '\0'; }			// remove the last character (',') from the result string
	SCPI_ResultCharacters(context, buffer, strlen(buffer));
	return SCPI_RES_OK;
}