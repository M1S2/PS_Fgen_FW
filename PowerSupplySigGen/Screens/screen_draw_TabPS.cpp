/*
 * screen_draw_TabPS.cpp
 *
 * Created: 02.09.2020 19:53:13
 *  Author: V17
 */ 

#include "screen_draw_TabPS.h"

/*Draw PS tab with voltage control and info texts*/
void Screen_DrawTabPS(u8g_t *u8g)
{
	double voltageSetting = 9.50;
	double outputEnabled = false;
	double measuredVoltage = 9.42;
	double measuredCurrent = 1.45;
	
	char stringBuffer[10];
	
	u8g_SetDefaultForegroundColor(u8g);
	
	/*Draw power supply voltage control*/
	u8g_SetFont(u8g, u8g_font_fur17r);	// 17 pixel height font
	dtostrf(voltageSetting, 5, 2, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, VOLTAGE_CONTROL_POSX, VOLTAGE_CONTROL_POSY, stringBuffer);
	
	/*Draw power supply voltage control*/
	u8g_SetFont(u8g, u8g_font_fur17r);	// 17 pixel height font
	if(outputEnabled) { strcpy(stringBuffer, "ON"); }
	else { strcpy(stringBuffer, "OFF"); }
	u8g_DrawStr(u8g, OUTPUT_STATE_CONTROL_POSX, OUTPUT_STATE_CONTROL_POSY, stringBuffer);

	/*Draw current info text*/
	u8g_SetFont(u8g, u8g_font_7x14r);	// 10 pixel height font
	dtostrf(measuredCurrent, 5, 2, stringBuffer);
	strcat(stringBuffer, " A");
	u8g_DrawStr(u8g, INFO_TEXTS_POSX, INFO_TEXT_CURRENT_POSY, stringBuffer);

	/*Draw voltage info text*/
	u8g_SetFont(u8g, u8g_font_7x14r);	// 10 pixel height font
	dtostrf(measuredVoltage, 5, 2, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, INFO_TEXTS_POSX, INFO_TEXT_VOLTAGE_POSY, stringBuffer);

	/*Draw power info text*/
	u8g_SetFont(u8g, u8g_font_7x14r);	// 10 pixel height font
	dtostrf(measuredVoltage * measuredCurrent, 5, 2, stringBuffer);
	strcat(stringBuffer, " W");
	u8g_DrawStr(u8g, INFO_TEXTS_POSX, INFO_TEXT_POWER_POSY, stringBuffer);
}