/*
 * SCPI_Commands.h
 *
 * Created: 20.11.2020 16:57:59
 *  Author: V17
 *
 * based on: https://github.com/Vrekrer/Vrekrer_scpi_parser
 */ 


#ifndef SCPI_COMMANDS_H_
#define SCPI_COMMANDS_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "SCPI_String_Array.h"

// Maximum number of valid tokens.
#ifndef SCPI_MAX_TOKENS
#define SCPI_MAX_TOKENS 100
#endif

// Maximum length of numeric suffixes (e.g. 3 means maximum of 3 decimal places).
#ifndef SCPI_MAX_SUFFIX_LENGTH
#define SCPI_MAX_SUFFIX_LENGTH 3
#endif

class SCPI_Commands : public SCPI_String_Array
{
	public:
		SCPI_Commands(char* message);
		char* not_processed_message;
		char numeric_suffixes[SCPI_MAX_TOKENS][SCPI_MAX_SUFFIX_LENGTH + 1];
};

typedef SCPI_Commands SCPI_C;

#endif /* SCPI_COMMANDS_H_ */