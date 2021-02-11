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
	return SCPI_SetNumericChannelParameter(context, SCPI_CHPARAM_AMPLITUDE);
}

scpi_result_t scpi_cmd_sourceVoltageLevelImmediateAmplitudeQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_AMPLITUDE);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceVoltageLevelImmediateOffset(scpi_t * context)
{
	return SCPI_SetNumericChannelParameter(context, SCPI_CHPARAM_OFFSET);
}

scpi_result_t scpi_cmd_sourceVoltageLevelImmediateOffsetQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_OFFSET);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceFrequencyFixed(scpi_t * context)
{
	return SCPI_SetNumericChannelParameter(context, SCPI_CHPARAM_FREQUENCY);
}

scpi_result_t scpi_cmd_sourceFrequencyFixedQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_FREQUENCY);
}

//----------------------------------------------------------------------------------------------------------

scpi_result_t scpi_cmd_sourceLoadImpedance(scpi_t * context)
{
	return SCPI_SetNumericChannelParameter(context, SCPI_CHPARAM_LOADIMPEDANCE);
}

scpi_result_t scpi_cmd_sourceLoadImpedanceQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_LOADIMPEDANCE);
}