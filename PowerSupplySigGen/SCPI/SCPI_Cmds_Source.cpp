/*
 * SCPI_Cmds_Source.cpp
 *
 * Created: 03.12.2020 18:35:51
 *  Author: V17
 */ 

#include "SCPI_Device.h"
#include "../Outputs/PowerSupply.h"

scpi_result_t scpi_cmd_sourceVoltage(scpi_t * context)
{
	int32_t sourceNumbers[1];
	SCPI_CommandNumbers(context, sourceNumbers, 1, 1);
	
	scpi_number_t param;
	if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &param, TRUE))
	{
		return SCPI_RES_ERR;
	}
	
	if(sourceNumbers[0] == 1)
	{
		float voltage = PowerSupply.Voltage;
		if (!SCPI_GetVoltageFromParam(context, param, voltage, 0, 10, 0, 1)) 
		{
			return SCPI_RES_ERR;
		}
		PowerSupply.SetVoltage(voltage);
	}
	else
	{
		const char* msg = "Channel not implemented yet.";
		SCPI_ResultCharacters(context, msg, strlen(msg));
	}
	
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_sourceVoltageQ(scpi_t * context)
{
	int32_t sourceNumbers[1];
	SCPI_CommandNumbers(context, sourceNumbers, 1, 1);
	
	if(sourceNumbers[0] == 1)
	{
		SCPI_ResultFloat(context, PowerSupply.Voltage);
	}
	return SCPI_RES_OK;
}