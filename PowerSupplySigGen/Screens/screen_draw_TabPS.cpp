/*
 * screen_draw_TabPS.cpp
 *
 * Created: 02.09.2020 19:53:13
 *  Author: V17
 */ 

#include "screen_draw_TabPS.h"

/*Draw PS tab with voltage control and info texts*/
void Screen_DrawTabPS(u8g_t *u8g, DevStatus_t devStatusDraw, DevSettings_t devSettingsDraw)
{	
	char stringBuffer[10];
	
	u8g_SetDefaultForegroundColor(u8g);
	u8g_SetFont(u8g, u8g_font_fur14r);	// 14 pixel height font
	
	/*Draw power supply voltage control*/
	dtostrf(devSettingsDraw.PS_Voltage, 6, 3, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, VOLTAGE_CONTROL_POSX, VOLTAGE_CONTROL_POSY, stringBuffer);
	
	/*Draw power supply voltage control*/
	if(devSettingsDraw.PS_Output_Enabled) { strcpy(stringBuffer, "ON"); }
	else { strcpy(stringBuffer, "OFF"); }
	u8g_DrawStr(u8g, OUTPUT_STATE_CONTROL_POSX, OUTPUT_STATE_CONTROL_POSY, stringBuffer);

	u8g_SetFont(u8g, u8g_font_7x14r);	// 10 pixel height font

	/*Draw current info text*/	
	dtostrf(devStatusDraw.PS_CURR, 6, 3, stringBuffer);
	strcat(stringBuffer, " A");
	u8g_DrawStr(u8g, INFO_TEXTS_POSX, INFO_TEXT_CURRENT_POSY, stringBuffer);

	/*Draw voltage info text*/
	dtostrf(devStatusDraw.PS_VOLT, 6, 3, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, INFO_TEXTS_POSX, INFO_TEXT_VOLTAGE_POSY, stringBuffer);

	/*Draw power info text*/
	dtostrf(devStatusDraw.PS_VOLT * devStatusDraw.PS_CURR, 6, 3, stringBuffer);
	strcat(stringBuffer, " W");
	u8g_DrawStr(u8g, INFO_TEXTS_POSX, INFO_TEXT_POWER_POSY, stringBuffer);
}