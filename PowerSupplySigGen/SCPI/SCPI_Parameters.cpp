/*
 * SCPI_Parameters.cpp
 *
 * Created: 20.11.2020 17:00:12
 *  Author: V17
 *
 * based on: https://github.com/Vrekrer/Vrekrer_scpi_parser
 */ 

#include "SCPI_Parameters.h"

SCPI_Parameters::SCPI_Parameters(char* message)
{
	if(message == NULL) { return; }
	char* parameter = message;
	// Discard parameters and multicommands
	this->not_processed_message = strpbrk(parameter, ";");
	if (this->not_processed_message != NULL)
	{
		this->not_processed_message += 1;
		parameter = strtok(parameter, ";");
		parameter = strtok(parameter, ",");
	}
	else
	{
		parameter = strtok(parameter, ",");
	}
	// Strip using ':'
	while (parameter != NULL)
	{
		while(isspace(*parameter)) parameter++;
		this->Append(parameter);
		parameter = strtok(NULL, ",");
	}
	//TODO add support for strings parameters (do not split parameters inside "")
}

bool SCPI_Parameters::GetParamBool(const uint8_t index)
{
	char* paramStr = values_[index];
	if (strcasecmp(paramStr, "1") == 0) { return true; }
	else if (strcasecmp(paramStr, "0") == 0) { return false; }
	else if (strcasecmp(paramStr, "ON") == 0) { return true; }
	else if (strcasecmp(paramStr, "OFF") == 0) { return false; }
	else
	{
		/* Invalid boolean parameter. */
	}
	return false;   /* Return false as default value */
}

int SCPI_Parameters::GetParamInt(const uint8_t index)
{
	char* paramStr = values_[index];
	return atoi(paramStr);
}

float SCPI_Parameters::GetParamFloat(const uint8_t index)
{
	char* paramStr = values_[index];
	return atof(paramStr);
}
