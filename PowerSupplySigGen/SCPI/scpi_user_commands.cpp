/*
 * scpi_user_commands.cpp
 *
 * Created: 17.11.2020 21:19:11
 *  Author: V17
 */ 

#include "SCPI_Parser.h"
#include "../Outputs/PowerSupply.h"
#include "../USART/USART.h"

void SCPIOutputState(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{		
	bool outputState;
	bool statusOk = parameters.GetParamBool(0, &outputState);
	if(statusOk)
	{
		if(outputState)
		{
			if(sendFunction != NULL) { sendFunction("OutputStateON\r\n"); }
			PowerSupply.OutputEnabled = true;
		}
		else
		{
			if(sendFunction != NULL) { sendFunction("OutputStateOFF\r\n"); }
			PowerSupply.OutputEnabled = false;
		}
		PowerSupply.UpdateOutput();
	}
}

void SCPIOutputVoltage(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	float outputVolt;
	bool statusOk = parameters.GetParamFloat(0, &outputVolt);
	if(statusOk)
	{
		if(sendFunction != NULL) { sendFunction("OutputVoltage\r\n"); }
		PowerSupply.Voltage = outputVolt;
		PowerSupply.UpdateOutput();
		/* Invalidate EEPROM values !!!!!!!!!!!! */
	}
}


void scpi_parser_init_user_commands()
{
	SCPIparser.RegisterCommand("OUTPut", &SCPIOutputState);
	SCPIparser.RegisterCommand("OUTPut:STATe", &SCPIOutputState);
	SCPIparser.RegisterCommand("OUTPut:VOLT", &SCPIOutputVoltage);
}