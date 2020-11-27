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

static char sbuf[MAX_ERROR_LEN + 1];

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
		bool appendStatus = this->Append(parameter);
		if (appendStatus)
		{
			parameter = strtok(NULL, ",");
		}
		else
		{
			parameter = NULL;
			/* Too much parameters for SCPI parser. */
			sprintf(sbuf, "The number of parameters exceeds the maximum number the parser is able to handle (max. %d).", SCPI_ARRAY_SIZE);
			SCPIparser.ErrorQueue.AddError(E_EXE_PARAMETER_ERROR, sbuf);
		}
	}
	//TODO add support for strings parameters (do not split parameters inside "")
}

bool SCPI_Parameters::GetParamBool(const uint8_t index, bool* value)
{
	char* paramStr = values_[index];
	if (strcasecmp(paramStr, "1") == 0) { *value = true; }
	else if (strcasecmp(paramStr, "0") == 0) { *value = false; }
	else if (strcasecmp(paramStr, "ON") == 0) { *value = true; }
	else if (strcasecmp(paramStr, "OFF") == 0) { *value = false; }
	else
	{
		/* Invalid boolean parameter. */
		sprintf(sbuf, "Invalid BOOL value: '%s'", paramStr);
		SCPIparser.ErrorQueue.AddError(E_CMD_DATA_TYPE_ERROR, sbuf);
		return false;
	}
	return true;
}

bool SCPI_Parameters::GetParamLong(const uint8_t index, long* value)
{
	char* paramStr = values_[index];
	char* endptr;
	*value = strtol(paramStr, &endptr, 10);
	if (endptr[0] != '\0')        // The whole number was valid, if the endptr only contains the string terminator
	{
		/* Invalid long parameter. */
		sprintf(sbuf, "Invalid LONG value: '%s'", paramStr);
		SCPIparser.ErrorQueue.AddError(E_CMD_INVALID_CHARACTER_IN_NUMBER, sbuf);
		return false;
	}
	return true;
}

bool SCPI_Parameters::GetParamFloat(const uint8_t index, float* value)
{
	char* paramStr = values_[index];
	char* endptr;
	*value = (float)strtod(paramStr, &endptr);
	if (endptr[0] != '\0')        // The whole number was valid, if the endptr only contains the string terminator
	{
		/* Invalid float parameter. */
		sprintf(sbuf, "Invalid FLOAT value: '%s'", paramStr);
		SCPIparser.ErrorQueue.AddError(E_CMD_INVALID_CHARACTER_IN_NUMBER, sbuf);
		return false;
	}
	return true;
}
