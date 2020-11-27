/*
 * SCPI_Commands.cpp
 *
 * Created: 20.11.2020 16:57:53
 *  Author: V17
 *
 * based on: https://github.com/Vrekrer/Vrekrer_scpi_parser
 */ 

#include "SCPI_Commands.h"
#include "scpi_parser.h"
#include <stdio.h>

static char sbuf[MAX_ERROR_LEN + 1];

SCPI_Commands::SCPI_Commands(char *message)
{
	if (message == NULL) { return; }
	char* token = message;
	// Trim leading spaces
	while (isspace(*token)) token++;
	// Discard parameters and multicommands
	this->not_processed_message = strpbrk(token, " \t;");
	if (this->not_processed_message != NULL)
	{
		this->not_processed_message += 1;
		token = strtok(token, " \t;");
		token = strtok(token, ":");
	}
	else
	{
		token = strtok(token, ":");
	}
	// Strip using ':'
	while (token != NULL)
	{
		bool appendStatus = this->Append(token);
		if (appendStatus)
		{
			token = strtok(NULL, ":");
		}
		else 
		{
			token = NULL; 
			/* Too much command tokens for SCPI parser. */
			sprintf(sbuf, "The number of command tokens exceeds the maximum number the parser is able to handle (max. %d).", SCPI_ARRAY_SIZE);
			SCPIparser.ErrorQueue.AddError(E_CMD_COMMAND_HEADER_ERROR, sbuf);
		}
	}
}
