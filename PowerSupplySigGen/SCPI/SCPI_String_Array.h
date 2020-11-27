/*
 * SCPI_String_Array.h
 *
 * Created: 20.11.2020 16:53:44
 *  Author: V17
 *
 * based on: https://github.com/Vrekrer/Vrekrer_scpi_parser
 */ 


#ifndef SCPI_STRING_ARRAY_H_
#define SCPI_STRING_ARRAY_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

// Maximum size of command tree and number of parameters.
#ifndef SCPI_ARRAY_SIZE
#define SCPI_ARRAY_SIZE 6
#endif

class SCPI_String_Array
{
	public:
		SCPI_String_Array() 
		{
			 size_ = 0; 
		}
	
		char* operator[](const uint8_t index);
		bool Append(char* value);
		char* Pop();
		char* First();
		char* Last();
		uint8_t Size();
	protected:
		uint8_t size_;
		char* values_[SCPI_ARRAY_SIZE];
};

#endif /* SCPI_STRING_ARRAY_H_ */