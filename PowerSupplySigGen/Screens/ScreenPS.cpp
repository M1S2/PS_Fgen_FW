/*
 * ScreenPS.cpp
 *
 * Created: 07.11.2020 13:09:35
 *  Author: V17
 */ 

#include "ScreenPS.h"

void ScreenPS::Draw(u8g_t* u8g, DevStatus_t devStatusDraw)
{
	char stringBuffer[10];
	
	u8g_SetDefaultForegroundColor(u8g);
	u8g_SetFont(u8g, u8g_font_7x14r);		// 10 pixel height font
	
#if 0
	/*Draw power supply voltage control*/
	dtostrf(devSettingsDraw.PS_Voltage_mV / 1000.0f, 6, 3, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, VOLTAGE_CONTROL_POSX, VOLTAGE_CONTROL_POSY, stringBuffer);
	u8g_DrawHLine(u8g, VOLTAGE_CONTROL_POSX + 21, VOLTAGE_CONTROL_POSY + 3, 6);
	
	/*Draw power supply voltage control*/
	if(devSettingsDraw.PS_Output_Enabled) { strcpy(stringBuffer, "ON"); }
	else { strcpy(stringBuffer, "OFF"); }
	u8g_DrawStr(u8g, OUTPUT_STATE_CONTROL_POSX, OUTPUT_STATE_CONTROL_POSY, stringBuffer);
	u8g_DrawFrame(u8g, OUTPUT_STATE_CONTROL_POSX - 3, OUTPUT_STATE_CONTROL_POSY - CONTROLS_FONT_HEIGHT - 3, 62, CONTROLS_FONT_HEIGHT + 6);
#endif

	/*Draw current info text*/
	dtostrf(devStatusDraw.PS_CURR_mV / 1000.0f, 6, 3, stringBuffer);
	strcat(stringBuffer, " A");
	u8g_DrawStr(u8g, INFO_TEXTS_POSX, INFO_TEXT_CURRENT_POSY, stringBuffer);

	/*Draw voltage info text*/
	dtostrf(devStatusDraw.PS_VOLT_mV / 1000.0f, 6, 3, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, INFO_TEXTS_POSX, INFO_TEXT_VOLTAGE_POSY, stringBuffer);

	/*Draw power info text*/
	dtostrf((devStatusDraw.PS_VOLT_mV / 1000.0f) * (devStatusDraw.PS_CURR_mV / 1000.0f), 6, 3, stringBuffer);
	strcat(stringBuffer, " W");
	u8g_DrawStr(u8g, INFO_TEXTS_POSX, INFO_TEXT_POWER_POSY, stringBuffer);
	
	ScreenBase::Draw(u8g, devStatusDraw);
}