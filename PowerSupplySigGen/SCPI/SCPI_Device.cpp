/*
 * SCPI_Device.cpp
 *
 * Created: 01.12.2020 18:08:13
 *  Author: V17
 */ 

#include "SCPI_Device.h"
#include "../USART/USART.h"
#include "../Global/DevSettings.h"
#include <stdio.h>

scpi_t scpi_context;
char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];
char resultBuffer[256];

const scpi_command_t scpi_commands[] = 
{
	/* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
	{"*CLS", SCPI_CoreCls, 0},
	{"*ESE", SCPI_CoreEse, 0},
	{"*ESE?", SCPI_CoreEseQ, 0},
	{"*ESR?", SCPI_CoreEsrQ, 0},
	{"*IDN?", SCPI_CoreIdnQ, 0},
	{"*OPC", SCPI_CoreOpc, 0},
	{"*OPC?", SCPI_CoreOpcQ, 0},
	{"*RST", SCPI_CoreRst, 0},
	{"*SRE", SCPI_CoreSre, 0},
	{"*SRE?", SCPI_CoreSreQ, 0},
	{"*STB?", SCPI_CoreStbQ, 0},
	//{"*TST?", My_CoreTstQ, 0},
	{"*WAI", SCPI_CoreWai, 0},
		
	/* Required SCPI commands (SCPI std V1999.0 4.2.1) */
	{"SYSTem:ERRor[:NEXT]?", SCPI_SystemErrorNextQ, 0},
	{"SYSTem:ERRor:COUNt?", SCPI_SystemErrorCountQ, 0},
	{"SYSTem:VERSion?", SCPI_SystemVersionQ, 0},
	
	{"STATus:QUEStionable[:EVENt]?", SCPI_StatusQuestionableEventQ, 0},
	{"STATus:QUEStionable:ENABle", SCPI_StatusQuestionableEnable, 0},
	{"STATus:QUEStionable:ENABle?", SCPI_StatusQuestionableEnableQ, 0},

	{"STATus:PRESet", SCPI_StatusPreset, 0},
		
	SCPI_CMD_LIST_END
};

scpi_interface_t scpi_interface = 
{
	/*.error = */ SCPI_Error,
	/*.write = */ SCPI_Write,
	/*.control = */ SCPI_Control,
	/*.flush = */ SCPI_Flush,
	/*.reset = */ SCPI_Reset,
};

size_t SCPI_Write(scpi_t * context, const char * data, size_t len) 
{
	(void) context;
	Usart0TransmitStr(data);
	return len;				/* OK */
	//return fwrite(data, 1, len, stdout);
}

scpi_result_t SCPI_Flush(scpi_t * context) 
{
	(void) context;
	return SCPI_RES_OK;
}

int SCPI_Error(scpi_t * context, int_fast16_t err) 
{
	(void) context;
	
	sprintf(resultBuffer, "**ERROR: %d, \"%s\"\r\n", (int16_t) err, SCPI_ErrorTranslate(err));
	Usart0TransmitStr(resultBuffer);
	return 0;
}

scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val) 
{
	(void) context;

	if (SCPI_CTRL_SRQ == ctrl) 
	{
		sprintf(resultBuffer, "**SRQ: 0x%X (%d)\r\n", val, val);
	} 
	else
	{
		sprintf(resultBuffer, "**CTRL %02x: 0x%X (%d)\r\n", ctrl, val, val);
	}
	Usart0TransmitStr(resultBuffer);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(scpi_t * context) 
{
	(void) context;
	ResetDevice();
	return SCPI_RES_OK;
}

void SCPI_Init_Device()
{
	SCPI_Init(&scpi_context,
				scpi_commands,
				&scpi_interface,
				scpi_units_def,
				SCPI_IDN1, SCPI_IDN2, SCPI_IDN3, SCPI_IDN4,
				scpi_input_buffer, SCPI_INPUT_BUFFER_LENGTH,
				scpi_error_queue_data, SCPI_ERROR_QUEUE_SIZE);
}