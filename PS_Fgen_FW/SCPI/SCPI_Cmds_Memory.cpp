/*
 * SCPI_Cmds_Memory.cpp
 * Created: 06.12.2020 17:09:22
 * Author: Markus Scheich
 */ 

#include "SCPI_Device.h"
#include "../Device.h"

scpi_result_t scpi_cmd_memoryRcl(scpi_t * context) 
{
	/* Only one profile supported */
	Device.LoadSettings();
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_memorySav(scpi_t * context)
{
	/* Only one profile supported */
	Device.SaveSettings();
	return SCPI_RES_OK;
}