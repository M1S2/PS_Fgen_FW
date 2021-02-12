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
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_OUTPUTSTATE);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_outputStateQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OUTPUTSTATE);
}

//**********************************************************************************************************

scpi_result_t scpi_cmd_outputGeneral(scpi_t * context)
{
	scpi_bool_t state;
	if(!SCPI_ParamBool(context, &state, TRUE))
	{
		return SCPI_RES_ERR;
	}
	
	for(int i = 0; i < NUM_CHANNELS; i++)
	{
		if (Device.Channels[i]->GetChannelType() == POWER_SUPPLY_CHANNEL_TYPE)
		{
			PS_Channel* psChannel = (PS_Channel*)Device.Channels[i];
			psChannel->SetEnabled(state);
		}
		else if (Device.Channels[i]->GetChannelType() == DDS_CHANNEL_TYPE)
		{
			DDS_Channel* ddsChannel = (DDS_Channel*)Device.Channels[i];
			ddsChannel->SetEnabled(state);
		}	
	}
	
	return SCPI_RES_OK;
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_outputGeneralQ(scpi_t * context)
{
	uint8_t outputStates[NUM_CHANNELS];
	uint8_t numProcessedOutputs = 0;
	for(int i = 0; i < NUM_CHANNELS; i++)
	{
		if (Device.Channels[i]->GetChannelType() == POWER_SUPPLY_CHANNEL_TYPE)
		{
			PS_Channel* psChannel = (PS_Channel*)Device.Channels[i];
			outputStates[numProcessedOutputs] = (psChannel->GetEnabled() ? 1 : 0);
			numProcessedOutputs++;
		}
		else if (Device.Channels[i]->GetChannelType() == DDS_CHANNEL_TYPE)
		{
			DDS_Channel* ddsChannel = (DDS_Channel*)Device.Channels[i];
			outputStates[numProcessedOutputs] = (ddsChannel->GetEnabled() ? 1 : 0);
			numProcessedOutputs++;
		}
	}
	
	SCPI_ResultArrayUInt8(context, outputStates, numProcessedOutputs, SCPI_FORMAT_ASCII);
	return SCPI_RES_OK;
}