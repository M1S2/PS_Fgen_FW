/*
 * SCPI_ErrorQueue.cpp
 *
 * Created: 20.11.2020 17:03:08
 *  Author: V17
 *
 * based on: https://github.com/MightyPork/scpi_parser/blob/master/source/scpi_errors.c
 */ 

#include "SCPI_ErrorQueue.h"
#include <stdio.h>

const SCPI_Error_Desc_t* SCPI_Error_Queue::findErrorDesc(const SCPI_Error_Desc_t* table, int16_t errorno)
{
    for(int i = 0; (i == 0 || table[i].errorno != 0); i++)
    {
		if (table[i].errorno == errorno) 
		{
			return &table[i];
		}
	}
	return NULL;
}

const SCPI_Error_Desc_t* SCPI_Error_Queue::resolveErrorDesc(int16_t errorno)
{
	const SCPI_Error_Desc_t* desc;

	if (errorno == 0) 
	{
		// ok state
		return &scpi_std_errors[0];
	}
	else if (errorno < 0) 
	{
		// standard errors

		desc = findErrorDesc(scpi_std_errors, errorno);
		if (desc != NULL) return desc;
		
		// not found in table, use group-common error
		errorno += -errorno % 10; // round to ten

		desc = findErrorDesc(scpi_std_errors, errorno);
		if (desc != NULL) return desc;

		errorno += -errorno % 100; // round to hundred

		desc = findErrorDesc(scpi_std_errors, errorno);
		if (desc != NULL) return desc;
	} 
	else 
	{
		// user error
		//desc = findErrorDesc(scpi_user_errors, errorno);
		//if (desc != NULL) return desc;
	}

	return NULL;
}

/**
 * Get error string.
 *
 * @param buffer Buffer for storing the final string. Make sure it's big enough.
 * @param errorno Error number
 * @param extra Extra information, appended after the generic message.
 *
 * @returns actual error code. Code may be coerced to closest defined code (categories: tens, hundreds)
 */
int16_t SCPI_Error_Queue::getErrorString(char *buffer, int16_t errorno, const char *extra)
{
    const SCPI_Error_Desc_t* desc = resolveErrorDesc(errorno);
	const char *msg;
	if (desc != NULL) 
	{
		errorno = desc->errorno;
		msg = desc->msg;
	}
	else 
	{
		// bad error code
		msg = "Unknown error";
	}

	// Print.
	if (extra == NULL) 
	{
		sprintf(buffer, "%d,\"%s\"", errorno, msg);
	} 
	else 
	{
		sprintf(buffer, "%d,\"%s; %s\"", errorno, msg, extra);
	}

	return errorno;
}

void SCPI_Error_Queue::AddError(int16_t errorno, const char *extra)
{
    if (_count >= ERR_QUEUE_LEN) 
    {
		errorno = E_DEV_QUEUE_OVERFLOW;
		extra = NULL;

		// backtrack
		_w_pos--;
		_count--;
		if (_w_pos < 0) 
		{
			_w_pos = ERR_QUEUE_LEN - 1;
		}
	}

	// get string & coerce errorno to valid value
	errorno = getErrorString(_queue[_w_pos], errorno, extra);

	_w_pos++;
	_count++;
	if (_w_pos >= ERR_QUEUE_LEN) 
	{
		_w_pos = 0;
	}

	// error type status flags
	if (errorno >= -499 && errorno <= -400) 
	{
		//SCPI_REG_SESR.QUERY_ERROR = true;
	} 
	else if ((errorno >= -399 && errorno <= -300) || errorno > 0) 
	{
		//SCPI_REG_SESR.DEV_ERROR = true;
	} 
	else if (errorno >= -299 && errorno <= -200) 
	{
		//SCPI_REG_SESR.EXE_ERROR = true;
	} 
	else if (errorno >= -199 && errorno <= -100) 
	{
		//SCPI_REG_SESR.CMD_ERROR = true;
	}
}

void SCPI_Error_Queue::ReadErrorNoRemove(char *buf)
{
    if (_count == 0) 
    {
		getErrorString(buf, E_NO_ERROR, NULL);
		return;
	}

	strcpy(buf, _queue[_r_pos]);
}

void SCPI_Error_Queue::ReadError(char *buf)
{
    if (_count == 0) 
    {
		getErrorString(buf, E_NO_ERROR, NULL);
		return;
	}

	strcpy(buf, _queue[_r_pos++]);
	_count--;

	if (_r_pos >= ERR_QUEUE_LEN) 
	{
		_r_pos = 0;
	}
}

void SCPI_Error_Queue::ClearErrors()
{
    _r_pos = 0;
	_w_pos = 0;
	_count = 0;
}

uint8_t SCPI_Error_Queue::ErrorCount()
{
    return _count;
}
