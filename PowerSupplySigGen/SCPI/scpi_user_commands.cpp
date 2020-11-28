/*
 * scpi_user_commands.cpp
 *
 * Created: 17.11.2020 21:19:11
 *  Author: V17
 */ 

#include "SCPI_Parser.h"
#include "../Outputs/PowerSupply.h"
#include "../Global/DevStatus.h"
#include <stdio.h>

void SCPIOutputState(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{		
	bool outputState;
	bool statusOk = parameters.GetParamBool(0, &outputState);
	if(statusOk)
	{
		PowerSupply.SetOutputEnabled(outputState);
	}
}

void SCPIOutputStateQuery(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL)
	{
		if(PowerSupply.OutputEnabled)
		{
			sendFunction("ON\r\n");
		}
		else
		{
			sendFunction("OFF\r\n");
		}
	}
}

void SCPISourceVoltage(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	float outputVolt;
	bool statusOk = parameters.GetParamFloat(0, &outputVolt);
	if(statusOk)
	{
		PowerSupply.SetVoltage(outputVolt);
	}
}

void SCPISourceVoltageQuery(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL) 
	{ 
		char buffer[15];
		dtostrf(PowerSupply.Voltage, 8, 3, buffer);
		sendFunction(buffer); 
		sendFunction("V\r\n");
	}
}

void SCPIMeasureVoltageQuery(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL)
	{
		char buffer[15];
		dtostrf(DevStatus.PS_VOLT, 8, 3, buffer);
		sendFunction(buffer);
		sendFunction("V\r\n");
	}
}

void SCPIMeasureCurrentQuery(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL)
	{
		char buffer[15];
		dtostrf(DevStatus.PS_CURR, 8, 3, buffer);
		sendFunction(buffer);
		sendFunction("A\r\n");
	}
}

void SCPIMeasurePowerQuery(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	if(sendFunction != NULL)
	{
		char buffer[15];
		dtostrf((DevStatus.PS_VOLT * DevStatus.PS_CURR), 8, 3, buffer);
		sendFunction(buffer);
		sendFunction("W\r\n");
	}
}

void scpi_parser_init_user_commands()
{
	/* Output subsystem */
	SCPIparser.RegisterCommand("OUTPut", &SCPIOutputState);
	SCPIparser.RegisterCommand("OUTPut?", &SCPIOutputStateQuery);
	SCPIparser.RegisterCommand("OUTPut:STATe", &SCPIOutputState);
	SCPIparser.RegisterCommand("OUTPut:STATe?", &SCPIOutputStateQuery);
	
	/* Source subsystem */
	SCPIparser.RegisterCommand("SOURce:VOLTage:LEVel", &SCPISourceVoltage);
	SCPIparser.RegisterCommand("SOURce:VOLTage:LEVel?", &SCPISourceVoltageQuery);
	SCPIparser.RegisterCommand("SOURce:VOLTage", &SCPISourceVoltage);
	SCPIparser.RegisterCommand("SOURce:VOLTage?", &SCPISourceVoltageQuery);
	SCPIparser.RegisterCommand("VOLTage", &SCPISourceVoltage);
	SCPIparser.RegisterCommand("VOLTage?", &SCPISourceVoltageQuery);
	
	/* Measure subsystem */
	SCPIparser.RegisterCommand("MEASure:VOLTage?", &SCPIMeasureVoltageQuery);
	SCPIparser.RegisterCommand("MEASure:CURRent?", &SCPIMeasureCurrentQuery);
	SCPIparser.RegisterCommand("MEASure:POWer?", &SCPIMeasurePowerQuery);
}