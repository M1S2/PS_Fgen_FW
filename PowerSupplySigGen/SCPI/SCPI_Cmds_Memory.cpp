/*
 * SCPI_Cmds_Memory.cpp
 *
 * Created: 06.12.2020 17:09:22
 *  Author: V17
 */ 

#include "SCPI_Device.h"
#include "../Global/DevSettings.h"

scpi_result_t scpi_cmd_memoryRcl(scpi_t * context) 
{
	/* Only one profile supported */
	LoadSettings();
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_memorySav(scpi_t * context)
{
	/* Only one profile supported */
	SaveSettings();
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_memoryNStatesQ(scpi_t * context)
{
	/* Only one profile supported */
	SCPI_ResultInt(context, 1);
	return SCPI_RES_OK;
}