/*
 * SCPI_Cmds_Display.cpp
 *
 * Created: 06.12.2020 18:56:13
 *  Author: V17
 */ 

#include "SCPI_Device.h"
#include "../Screens/ScreenManager.h"

scpi_result_t scpi_cmd_displayWindowState(scpi_t * context)
{
	scpi_bool_t enabled;
	if(!SCPI_ParamBool(context, &enabled, TRUE))
	{
		return SCPI_RES_ERR;
	}
	
	ScreenManager.SetDisplayEnabled(enabled);
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_displayWindowStateQ(scpi_t * context)
{
	SCPI_ResultBool(context, ScreenManager.GetDisplayEnabled());
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_displayWindowText(scpi_t * context)
{
	char text[MAX_MESSAGE_LEN];
	size_t textLen;
	if(!SCPI_ParamCopyText(context, text, sizeof(text), &textLen, TRUE))
	{
		return SCPI_RES_ERR;
	}
	strcpy(ScreenManager.UserMessage, text);
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_displayWindowTextQ(scpi_t * context)
{
	SCPI_ResultCharacters(context, ScreenManager.UserMessage, strlen(ScreenManager.UserMessage));
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_displayWindowClear(scpi_t * context)
{
	strcpy(ScreenManager.UserMessage, "");
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_displayWindowInverse(scpi_t * context)
{
	scpi_bool_t inverted;
	if(!SCPI_ParamBool(context, &inverted, TRUE))
	{
		return SCPI_RES_ERR;
	}
	
	ScreenManager.SetDisplayInverted(inverted);
	return SCPI_RES_OK;
}

scpi_result_t scpi_cmd_displayWindowInverseQ(scpi_t * context)
{
	SCPI_ResultBool(context, ScreenManager.DisplayInverted);
	return SCPI_RES_OK;
}