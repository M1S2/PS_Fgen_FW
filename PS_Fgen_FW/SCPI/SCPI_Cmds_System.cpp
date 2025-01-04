/*
 * SCPI_Cmds_System.cpp
 * Created: 06.12.2020 14:50:38
 * Author: Markus Scheich
 */ 

#include "SCPI_Device.h"
#include "../Device.h"

scpi_result_t scpi_cmd_systemLocal(scpi_t * context)
{
	Device.SetDeviceControlState(DEV_CTRL_LOCAL);
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemRemote(scpi_t * context)
{
	Device.SetDeviceControlState(DEV_CTRL_REMOTE);
	return SCPI_RES_OK;	
}

scpi_result_t scpi_cmd_systemRWLock(scpi_t * context)
{
	Device.SetDeviceControlState(DEV_CTRL_RWLOCK);
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemChannelCountQ(scpi_t * context)
{
	SCPI_ResultInt32(context, NUM_CHANNELS);
	return SCPI_RES_OK;
}

/***** CPU ******************************************************************************************************************************/

scpi_result_t scpi_cmd_systemFirmwareQ(scpi_t * context)
{
	SCPI_ResultCharacters(context, SCPI_IDN_SOFTWARE_REVISION, strlen(SCPI_IDN_SOFTWARE_REVISION));
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemSnoQ(scpi_t * context)
{
	SCPI_ResultCharacters(context, SCPI_IDN_SERIAL_NUMBER, strlen(SCPI_IDN_SERIAL_NUMBER));
	return SCPI_RES_OK;
}

/***** Measurement **********************************************************************************************************************/

scpi_result_t scpi_cmd_systemMeasureScalarVoltageDCQ(scpi_t * context)
{
	float voltages[2] = { Device.DeviceVoltages.ATX_3V3, Device.DeviceVoltages.ATX_5V };
	SCPI_ResultArrayFloat(context, voltages, 2, SCPI_FORMAT_ASCII);
	return SCPI_RES_OK;
}