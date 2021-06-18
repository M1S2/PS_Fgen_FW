/*
 * SCPI_Cmds_Source.cpp
 *
 * Created: 03.12.2020 18:35:51
 *  Author: V17
 */ 

#include "SCPI_Device.h"
#include "../Device.h"

scpi_result_t scpi_cmd_sourceVoltageLevelImmediateAmplitude(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_AMPLITUDE);
}

scpi_result_t scpi_cmd_sourceVoltageLevelImmediateAmplitudeQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_AMPLITUDE);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceVoltageLevelImmediateOffset(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_OFFSET);
}

scpi_result_t scpi_cmd_sourceVoltageLevelImmediateOffsetQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OFFSET);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceVoltageProtectionLevel(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_OVP_LEVEL);
}

scpi_result_t scpi_cmd_sourceVoltageProtectionLevelQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OVP_LEVEL);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceVoltageProtectionState(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_OVP_STATE);
}

scpi_result_t scpi_cmd_sourceVoltageProtectionStateQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OVP_STATE);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceVoltageProtectionDelay(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_OVP_DELAY);
}

scpi_result_t scpi_cmd_sourceVoltageProtectionDelayQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OVP_DELAY);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceVoltageProtectionTrippedQ(scpi_t * context)
{
	int32_t sourceNumbers[1];
	SCPI_CommandNumbers(context, sourceNumbers, 1, Device.SelectedScpiChannelIndex);
	
	int32_t channelNum = sourceNumbers[0];
	if(channelNum < 0 || channelNum >= NUM_CHANNELS)
	{
		return SCPI_SetResult_ChannelOutOfRange(context);
	}
	
	if (Device.Channels[channelNum]->GetChannelType() == POWER_SUPPLY_CHANNEL_TYPE)
	{
		PS_Channel* psChannel = (PS_Channel*)Device.Channels[channelNum];
		SCPI_ResultBool(context, (psChannel->GetPsState() == PS_STATE_OVP));
	}
	else
	{
		return SCPI_SetResult_NotSupportedByChannel(context);
	}
	return SCPI_RES_OK;
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceVoltageProtectionClear(scpi_t * context)
{
	int32_t sourceNumbers[1];
	SCPI_CommandNumbers(context, sourceNumbers, 1, Device.SelectedScpiChannelIndex);
	
	int32_t channelNum = sourceNumbers[0];
	if(channelNum < 0 || channelNum >= NUM_CHANNELS)
	{
		return SCPI_SetResult_ChannelOutOfRange(context);
	}
	
	if (Device.Channels[channelNum]->GetChannelType() == POWER_SUPPLY_CHANNEL_TYPE)
	{
		PS_Channel* psChannel = (PS_Channel*)Device.Channels[channelNum];
		psChannel->ClearProtections();
	}
	else
	{
		return SCPI_SetResult_NotSupportedByChannel(context);
	}
	return SCPI_RES_OK;
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceCurrentLevelImmediateAmplitude(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_CURRENT);
}

scpi_result_t scpi_cmd_sourceCurrentLevelImmediateAmplitudeQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_CURRENT);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceCurrentProtectionLevel(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_OCP_LEVEL);
}

scpi_result_t scpi_cmd_sourceCurrentProtectionLevelQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OCP_LEVEL);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceCurrentProtectionState(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_OCP_STATE);
}

scpi_result_t scpi_cmd_sourceCurrentProtectionStateQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OCP_STATE);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceCurrentProtectionDelay(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_OCP_DELAY);
}

scpi_result_t scpi_cmd_sourceCurrentProtectionDelayQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OCP_DELAY);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceCurrentProtectionTrippedQ(scpi_t * context)
{
	int32_t sourceNumbers[1];
	SCPI_CommandNumbers(context, sourceNumbers, 1, Device.SelectedScpiChannelIndex);
	
	int32_t channelNum = sourceNumbers[0];
	if(channelNum < 0 || channelNum >= NUM_CHANNELS)
	{
		return SCPI_SetResult_ChannelOutOfRange(context);
	}
	
	if (Device.Channels[channelNum]->GetChannelType() == POWER_SUPPLY_CHANNEL_TYPE)
	{
		PS_Channel* psChannel = (PS_Channel*)Device.Channels[channelNum];
		SCPI_ResultBool(context, (psChannel->GetPsState() == PS_STATE_OCP));
	}
	else
	{
		return SCPI_SetResult_NotSupportedByChannel(context);
	}
	return SCPI_RES_OK;
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceCurrentProtectionClear(scpi_t * context)
{
	/* It is only supported to clear all protections together. So no differences to VoltageProtectionClear function exists. */
	return scpi_cmd_sourceVoltageProtectionClear(context);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourcePowerProtectionLevel(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_OPP_LEVEL);
}

scpi_result_t scpi_cmd_sourcePowerProtectionLevelQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OPP_LEVEL);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourcePowerProtectionState(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_OPP_STATE);
}

scpi_result_t scpi_cmd_sourcePowerProtectionStateQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OPP_STATE);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourcePowerProtectionDelay(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_OPP_DELAY);
}

scpi_result_t scpi_cmd_sourcePowerProtectionDelayQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OPP_DELAY);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourcePowerProtectionTrippedQ(scpi_t * context)
{
	int32_t sourceNumbers[1];
	SCPI_CommandNumbers(context, sourceNumbers, 1, Device.SelectedScpiChannelIndex);
	
	int32_t channelNum = sourceNumbers[0];
	if(channelNum < 0 || channelNum >= NUM_CHANNELS)
	{
		return SCPI_SetResult_ChannelOutOfRange(context);
	}
	
	if (Device.Channels[channelNum]->GetChannelType() == POWER_SUPPLY_CHANNEL_TYPE)
	{
		PS_Channel* psChannel = (PS_Channel*)Device.Channels[channelNum];
		SCPI_ResultBool(context, (psChannel->GetPsState() == PS_STATE_OPP));
	}
	else
	{
		return SCPI_SetResult_NotSupportedByChannel(context);
	}
	return SCPI_RES_OK;
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourcePowerProtectionClear(scpi_t * context)
{
	/* It is only supported to clear all protections together. So no differences to VoltageProtectionClear function exists. */
	return scpi_cmd_sourceVoltageProtectionClear(context);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceFrequencyFixed(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_FREQUENCY);
}

scpi_result_t scpi_cmd_sourceFrequencyFixedQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_FREQUENCY);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceFunctionShape(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_SIGNALFORM);
}

scpi_result_t scpi_cmd_sourceFunctionShapeQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_SIGNALFORM);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceFunctionModeQ(scpi_t * context)
{
	char buffer[20];
	strcpy(buffer, "VOLTage");
	SCPI_ResultCharacters(context, buffer, strlen(buffer));
	return SCPI_RES_OK;
}

//----------------------------------------------------------------------------------------------------------

#ifdef DDS_USER_DEFINED_WAVEFORMS_ENABLED
scpi_result_t scpi_cmd_sourceFunctionData(scpi_t * context)
{
	return SCPI_SetChannelParameter(context, SCPI_CHPARAM_USERWAVEFORMDATA);
}
#endif