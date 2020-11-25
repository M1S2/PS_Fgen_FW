/*
 * SCPI_Parameters.h
 *
 * Created: 20.11.2020 16:59:59
 *  Author: V17
 *
 * based on: https://github.com/Vrekrer/Vrekrer_scpi_parser
 */ 


#ifndef SCPI_PARAMETERS_H_
#define SCPI_PARAMETERS_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "SCPI_String_Array.h"

class SCPI_Parameters : public SCPI_String_Array
{
	public:
		SCPI_Parameters(char *message);
		
		int GetParamBool(const uint8_t index, bool* value);
		int GetParamInt(const uint8_t index, int* value);
		int GetParamFloat(const uint8_t index, float* value);
		
		char* not_processed_message;
};

typedef SCPI_Parameters SCPI_P;

#endif /* SCPI_PARAMETERS_H_ */