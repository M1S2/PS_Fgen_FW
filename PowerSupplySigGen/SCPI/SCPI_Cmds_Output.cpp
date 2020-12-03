/*
 * SCPI_Cmds_Output.cpp
 *
 * Created: 03.12.2020 18:16:17
 *  Author: V17
 */ 

#include "SCPI_Device.h"
#include "../Outputs/PowerSupply.h"

scpi_result_t scpi_cmd_outputState(scpi_t * context)
{
	int32_t numbers[1];
	SCPI_CommandNumbers(context, numbers, 1, 1);
	
	scpi_bool_t state;
	if(!SCPI_ParamBool(context, &state, TRUE))
	{
		return SCPI_RES_ERR;
	}
	
	if(numbers[0] == 1)
	{
		PowerSupply.SetOutputEnabled(state);
	}
	else
	{
		const char* msg = "Channel not implemented yet.";
		SCPI_ResultCharacters(context, msg, strlen(msg));
	}
	
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_outputStateQ(scpi_t * context)
{
	int32_t numbers[1];
	SCPI_CommandNumbers(context, numbers, 1, 1);
	
	if(numbers[0] == 1)
	{
		SCPI_ResultBool(context, PowerSupply.OutputEnabled);
	}
	return SCPI_RES_OK;
}