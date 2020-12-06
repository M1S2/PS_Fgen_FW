/*
 * SCPI_Cmds_Display.cpp
 *
 * Created: 06.12.2020 18:56:13
 *  Author: V17
 */ 

#include "SCPI_Device.h"
#include "../Screens/ScreenManager.h"

scpi_result_t scpi_cmd_displayEnable(scpi_t * context)
{
	scpi_bool_t enabled;
	if(!SCPI_ParamBool(context, &enabled, TRUE))
	{
		return SCPI_RES_ERR;
	}
	
	ScreenManager.SetDisplayEnabled(enabled);
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_displayEnableQ(scpi_t * context)
{
	SCPI_ResultBool(context, ScreenManager.GetDisplayEnabled());
	return SCPI_RES_OK;
}