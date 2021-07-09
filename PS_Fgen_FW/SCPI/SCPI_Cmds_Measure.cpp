/*
 * SCPI_Cmds_Measure.cpp
 * Created: 12.02.2021 17:57:24
 * Author: Markus Scheich
 */ 

#include "SCPI_Device.h"
#include "../Device.h"

scpi_result_t scpi_cmd_measureScalarCurrentDCQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_MEASURED_CURRENT);
}

scpi_result_t scpi_cmd_measureScalarPowerDCQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_MEASURED_POWER);
}

scpi_result_t scpi_cmd_measureScalarVoltageDCQ(scpi_t * context)
{
	return SCPI_QueryChannelParameter(context, SCPI_CHPARAM_MEASURED_AMPLITUDE);
}