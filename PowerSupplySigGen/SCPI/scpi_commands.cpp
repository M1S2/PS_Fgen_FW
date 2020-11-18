/*
 * scpi_commands.cpp
 *
 * Created: 17.11.2020 21:19:11
 *  Author: V17
 */ 

#include "scpi_parser.h"
#include "../USART/USART.h"
#include "../Outputs/PowerSupply.h"

void Identify(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{
	char IDN[] = "Markus Scheich,PowerSupplySigGen,0,v1.0\n";
	if(sendFunction != NULL)
	{
		sendFunction(IDN);
	}
}

void OutputState(SCPI_C commands, SCPI_P parameters, SCPI_send_str_t sendFunction)
{		
	char *paramState = parameters.First();
	if(paramState == NULL) { paramState = ""; }
		
	if(paramState == "ON")
	{
		if(sendFunction != NULL) { sendFunction("OutputStateON\n"); }
		PowerSupply.OutputEnabled = true;
	}
	else
	{
		if(sendFunction != NULL) { sendFunction("OutputStateOFF\n"); }
		PowerSupply.OutputEnabled = false;
	}
	PowerSupply.UpdateOutput();
}


void scpi_parser_init_commands()
{
	 SCPIparser.SendStrFunction = &Usart0TransmitStr;
	 SCPIparser.RegisterCommand("*IDN?", &Identify);
     SCPIparser.RegisterCommand("OUTPut", &OutputState);

}