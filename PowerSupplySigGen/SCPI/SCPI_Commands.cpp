/*
 * SCPI_Commands.cpp
 *
 * Created: 20.11.2020 16:57:53
 *  Author: V17
 *
 * based on: https://github.com/Vrekrer/Vrekrer_scpi_parser
 */ 

#include "SCPI_Commands.h"

SCPI_Commands::SCPI_Commands(char *message)
{
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
		this->Append(token);
		token = strtok(NULL, ":");
	}
}
