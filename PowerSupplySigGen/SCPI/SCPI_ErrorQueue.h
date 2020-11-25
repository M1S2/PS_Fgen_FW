/*
 * SCPI_ErrorQueue.h
 *
 * Created: 20.11.2020 17:03:01
 *  Author: V17
 *
 * based on: https://github.com/MightyPork/scpi_parser/blob/master/source/scpi_errors.c
 */ 


#ifndef SCPI_ERRORQUEUE_H_
#define SCPI_ERRORQUEUE_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define ERR_QUEUE_LEN 4
#define MAX_ERROR_LEN 150

typedef struct SCPI_Error_Desc
{
	int16_t errorno;
	const char *msg;
	SCPI_Error_Desc(int16_t _errorno, const char* _msg)
	{
		errorno = _errorno;
		msg = _msg;
	}
} SCPI_Error_Desc_t;

class SCPI_Error_Queue
{
	public:
		SCPI_Error_Queue()
		{
			_r_pos = 0;
			_w_pos = 0;
			_count = 0;
		}
		
		void AddError(int16_t errorno, const char *extra);
		void ReadErrorNoRemove(char *buf);
		void ReadError(char *buf);
		void ClearErrors();
		int8_t ErrorCount();
		
	private:
		int16_t getErrorString(char *buffer, int16_t errorno, const char *extra);
		const SCPI_Error_Desc_t* resolveErrorDesc(int16_t errorno);
		const SCPI_Error_Desc_t* findErrorDesc(const SCPI_Error_Desc_t* table, int16_t errorno);
	
		char _queue[ERR_QUEUE_LEN][MAX_ERROR_LEN + 1];
		int8_t _r_pos;
		int8_t _w_pos;
		int8_t _count;	// signed for backtracking
};

typedef SCPI_Error_Queue SCPI_ERR;

// SCPI error constants
enum SCPI_Error_Constants
{
	E_NO_ERROR = 0,
	E_COMMAND_ERROR = -100,
	E_CMD_COMMAND_HEADER_ERROR = -110,
	E_CMD_NUMERIC_DATA_ERROR = -120,
	E_CMD_SUFFIX_ERROR = -130,
	E_CMD_CHARACTER_DATA_ERROR = -140,
	E_CMD_STRING_DATA_ERROR = -150,
	E_CMD_BLOCK_DATA_ERROR = -160,
	E_CMD_EXPRESSION_ERROR = -170,
	E_CMD_MACRO_ERROR = -180,
	E_EXECUTION_ERROR = -200,
	E_EXE_TRIGGER_ERROR = -210,
	E_EXE_PARAMETER_ERROR = -220,
	E_EXE_DATA_CORRUPT_OR_STALE = -230,
	E_EXE_HARDWARE_ERROR = -240,
	E_EXE_MASS_STORAGE_ERROR = -250,
	E_EXE_EXPRESSION_ERROR = -260,
	E_EXE_MACRO_ERROR = -270,
	E_EXE_PROGRAM_ERROR = -280,
	E_EXE_MEMORY_USE_ERROR = -290,
	E_DEVICE_SPECIFIC_ERROR = -300,
	E_DEV_SYSTEM_ERROR = -310,
	E_DEV_STORAGE_FAULT = -320,
	E_DEV_SELF_TEST_FAILED = -330,
	E_DEV_CALIBRATION_FAILED = -340,
	E_DEV_QUEUE_OVERFLOW = -350,
	E_DEV_COMMUNICATION_ERROR = -360,
	E_QUERY_ERROR = -400
};

static const SCPI_Error_Desc_t scpi_std_errors[] =
{
	{ 0, "No error"},
	{ -100, "Command error"},
	{ -110, "Command header error"},
	{ -120, "Numeric data error"},
	{ -130, "Suffix error"},
	{ -140, "Character data error"},
	{ -150, "String data error"},
	{ -160, "Block data error"},
	{ -170, "Expression error"},
	{ -180, "Macro error"},
	{ -200, "Execution error"},

	{ -210, "Trigger error"},
	{ -220, "Parameter error"},
	{ -230, "Data corrupt or stale"},
	{ -240, "Hardware error"},
	{ -250, "Mass storage error"},
	{ -260, "Expression error"},
	{ -270, "Macro error"},
	{ -280, "Program error"},
	{ -290, "Memory use error"},

	{ -300, "Device-specific error"},
	{ -310, "System error"},
	{ -320, "Storage fault"},
	{ -330, "Self-test failed"},
	{ -340, "Calibration failed"},
	{ -350, "Queue overflow"},
	{ -360, "Communication error"},

	{ -400, "Query error"},
	{ 0, ""}    /* END marker */
};

#endif /* SCPI_ERRORQUEUE_H_ */