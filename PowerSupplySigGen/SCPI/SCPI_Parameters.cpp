/*
 * SCPI_Parameters.cpp
 *
 * Created: 20.11.2020 17:00:12
 *  Author: V17
 *
 * based on: https://github.com/Vrekrer/Vrekrer_scpi_parser
 */ 

#include "SCPI_Parameters.h"
#include "scpi_parser.h"
#include <stdio.h>

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

int SCPI_Parameters::GetParamBool(const uint8_t index, bool* value)
{
	char* paramStr = values_[index];
	if (strcasecmp(paramStr, "1") == 0) { *value = true; }
	else if (strcasecmp(paramStr, "0") == 0) { *value = false; }
	else if (strcasecmp(paramStr, "ON") == 0) { *value = true; }
	else if (strcasecmp(paramStr, "OFF") == 0) { *value = false; }
	else
	{
		/* Invalid boolean parameter. */
		char buf[50];
		sprintf(buf, "Invalid BOOL value: '%s'", paramStr);
		SCPIparser.ErrorQueue.AddError(E_CMD_NUMERIC_DATA_ERROR, buf);
		return 1;
	}
	return 0;
}

int SCPI_Parameters::GetParamInt(const uint8_t index, int* value)
{
	char* paramStr = values_[index];
	*value = atoi(paramStr);
	return 0;
}

int SCPI_Parameters::GetParamFloat(const uint8_t index, float* value)
{
	char* paramStr = values_[index];
	*value = atof(paramStr);
	return 0;
}
