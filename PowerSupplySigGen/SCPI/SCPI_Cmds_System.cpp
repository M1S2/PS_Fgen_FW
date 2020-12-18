/*
 * SCPI_Cmds_System.cpp
 *
 * Created: 06.12.2020 14:50:38
 *  Author: V17
 */ 

#include "SCPI_Device.h"
#include "../Device.h"
#include "../Screens/ScreenManager.h"

scpi_result_t scpi_cmd_systemLocal(scpi_t * context)
{
	Device.DeviceControlState = DEV_CTRL_LOCAL;
	strcpy(ScreenManager.SystemMessage, "");
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemRemote(scpi_t * context)
{
	Device.DeviceControlState = DEV_CTRL_REMOTE;
	strcpy(ScreenManager.SystemMessage, "");
	return SCPI_RES_OK;	
}

scpi_result_t scpi_cmd_systemRWLock(scpi_t * context)
{
	Device.DeviceControlState = DEV_CTRL_RWLOCK;
	strcpy(ScreenManager.SystemMessage, "==== Remote controlled ====\nDevice locked by SYST:RWL command.\nOnly possible to unlock via SYST:LOC command.");
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemChannelCountQ(scpi_t * context)
{
	SCPI_ResultInt(context, NUM_OUTPUT_CHANNELS);
	return SCPI_RES_OK;
}

/***** CPU ******************************************************************************************************************************/

scpi_result_t scpi_cmd_systemCPUFirmwareQ(scpi_t * context)
{
	SCPI_ResultCharacters(context, SCPI_IDN_SOFTWARE_REVISION, strlen(SCPI_IDN_SOFTWARE_REVISION));
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemCPUInformationTypeQ(scpi_t * context)
{
	SCPI_ResultCharacters(context, CPU_TYPE, strlen(CPU_TYPE));
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemCPUSnoQ(scpi_t * context)
{
	SCPI_ResultCharacters(context, SCPI_IDN_SERIAL_NUMBER, strlen(SCPI_IDN_SERIAL_NUMBER));
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemCPUVersionQ(scpi_t * context)
{
	SCPI_ResultCharacters(context, CPU_BOARD_VERSION, strlen(CPU_BOARD_VERSION));
	return SCPI_RES_OK;
}

/***** Communication Serial *************************************************************************************************************/

scpi_result_t scpi_cmd_systemCommunicateSerialBaud(scpi_t * context)
{
	uint32_t baud;	
	if(!SCPI_ParamUInt32(context, &baud, TRUE))
	{
		return SCPI_RES_ERR;
	}
	Device.SetSerialBaudRate(baud);
	
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemCommunicateSerialBaudQ(scpi_t * context)
{
	SCPI_ResultInt(context, Device.SerialBaudRate);
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemCommunicateSerialEcho(scpi_t * context)
{
	scpi_bool_t enabled;
	if(!SCPI_ParamBool(context, &enabled, TRUE))
	{
		return SCPI_RES_ERR;
	}
	Device.SetSerialEchoEnabled(enabled);
	
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemCommunicateSerialEchoQ(scpi_t * context)
{
	SCPI_ResultBool(context, Device.SerialEchoEnabled);
	return SCPI_RES_OK;
}
