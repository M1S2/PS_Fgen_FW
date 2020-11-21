/*
 * SCPI_String_Array.cpp
 *
 * Created: 20.11.2020 16:53:33
 *  Author: V17
 *
 * based on: https://github.com/Vrekrer/Vrekrer_scpi_parser
 */ 

#include "SCPI_String_Array.h"

char* SCPI_String_Array::operator[](const uint8_t index)
{
	return values_[index];
}

void SCPI_String_Array::Append(char* value)
{
	if (size_ < SCPI_ARRAY_SIZE)
	{
		values_[size_] = value;
		size_++;
	}
}

char* SCPI_String_Array::Pop()
{
	if (size_ > 0)
	{
		size_--;
		return values_[size_];
	}
	else
	{
		return NULL;
	}
}

char* SCPI_String_Array::First()
{
	if (size_ > 0)
	{
		return values_[0];
	}
	else
	{
		return NULL;
	}
}

char* SCPI_String_Array::Last()
{
	if (size_ > 0)
	{
		return values_[size_ - 1];
	}
	else
	{
		return NULL;
	}
}

uint8_t SCPI_String_Array::Size()
{
	return size_;
}
