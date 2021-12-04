/*
 * SCPI_Cmds_System.cpp
 * Created: 06.12.2020 14:50:38
 * Author: Markus Scheich
 */ 

#include "SCPI_Device.h"
#include "../Device.h"

static scpi_choice_def_t baudrate_choice[] =
{
	{ "B110", 110 },
	{ "B150", 150 },
	{ "B300", 300 },
	{ "B1200", 1200 },
	{ "B2400", 2400 },
	{ "B4800", 4800 },
	{ "B9600", 9600 },
	{ "B19200", 19200 },
	{ "B38400", 38400 },
	{ "B57600", 57600 },
	SCPI_CHOICE_LIST_END /* termination of option list */
};


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

scpi_result_t scpi_cmd_systemCapabilityQ(scpi_t * context)
{
	SCPI_ResultCharacters(context, SCPI_SYSTEM_CAPABILITIES, strlen(SCPI_SYSTEM_CAPABILITIES));
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemPonOutputDisable(scpi_t * context)
{
	scpi_bool_t disabled;
	if(!SCPI_ParamBool(context, &disabled, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(disabled && Device.PowerOnOutputsState != DEV_POWERUP_OUTPUTS_OFF)
	{
		Device.PowerOnOutputsState = DEV_POWERUP_OUTPUTS_OFF;
		Device.SetSettingsChanged(true); 
	}
	else if(!disabled && Device.PowerOnOutputsState != DEV_POWERUP_OUTPUTS_LASTSTATE)
	{
		Device.PowerOnOutputsState = DEV_POWERUP_OUTPUTS_LASTSTATE;
		Device.SetSettingsChanged(true);
	}
	
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemPonOutputDisableQ(scpi_t * context)
{
	SCPI_ResultBool(context, Device.PowerOnOutputsState == DEV_POWERUP_OUTPUTS_OFF);
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

/***** Communication Serial *************************************************************************************************************/

scpi_result_t scpi_cmd_systemCommunicateSerialBaud(scpi_t * context)
{
	int32_t baudRate;
	if (!SCPI_ParamChoice(context, baudrate_choice, &baudRate, TRUE))
	{
		return SCPI_RES_ERR;
	}
	
	DeviceBaudRates_t baudRateEnum = DEV_BAUD_9600;
	switch(baudRate)
	{
		case 110: baudRateEnum = DEV_BAUD_110; break;
		case 150: baudRateEnum = DEV_BAUD_150; break;
		case 300: baudRateEnum = DEV_BAUD_300; break;
		case 1200: baudRateEnum = DEV_BAUD_1200; break;
		case 2400: baudRateEnum = DEV_BAUD_2400; break;
		case 4800: baudRateEnum = DEV_BAUD_4800; break;
		case 9600: baudRateEnum = DEV_BAUD_9600; break;
		case 19200: baudRateEnum = DEV_BAUD_19200; break;
		case 38400: baudRateEnum = DEV_BAUD_38400; break;
		case 57600: baudRateEnum = DEV_BAUD_57600; break;
		default: break;
	}
	Device.SetSerialBaudRate(baudRateEnum);
	
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_systemCommunicateSerialBaudQ(scpi_t * context)
{
	const char* buffer;
	if (!SCPI_ChoiceToName(baudrate_choice, (int32_t)DeviceBaudRateEnumToNumber(Device.SerialBaudRate), &buffer)) 
	{
		return SCPI_RES_ERR;
	}	
	SCPI_ResultCharacters(context, buffer, strlen(buffer));
	
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

/***** Measurement **********************************************************************************************************************/

scpi_result_t scpi_cmd_systemMeasureScalarVoltageDCQ(scpi_t * context)
{
	float voltages[4] = { Device.DeviceVoltages.ATX_3V3, Device.DeviceVoltages.ATX_5V, Device.DeviceVoltages.ATX_12V, Device.DeviceVoltages.ATX_12V_NEG };
	SCPI_ResultArrayFloat(context, voltages, 4, SCPI_FORMAT_ASCII);
	return SCPI_RES_OK;
}