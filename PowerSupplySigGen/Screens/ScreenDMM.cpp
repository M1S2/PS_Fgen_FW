/*
 * ScreenDMM.cpp
 *
 * Created: 07.11.2020 13:15:21
 *  Author: V17
 */ 

#include "ScreenDMM.h"

void ScreenDMM::Draw(u8g_t* u8g, DevStatus_t devStatusDraw)
{
	char stringBuffer[10];
	
	u8g_SetDefaultForegroundColor(u8g);
	u8g_SetFont(u8g, u8g_font_7x14r);	// 10 pixel height font
	
	/*Draw DMM1 info text*/
	dtostrf(devStatusDraw.DMM1_mV / 1000.0f, 6, 3, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, DMM_COLUMN1_POSX, DMM_ROW1_POSY, "DMM1: ");
	u8g_DrawStr(u8g, DMM_COLUMN2_POSX, DMM_ROW1_POSY, stringBuffer);
	
	/*Draw DMM2 info text*/
	dtostrf(devStatusDraw.DMM2_mV / 1000.0f, 6, 3, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, DMM_COLUMN1_POSX, DMM_ROW2_POSY, "DMM2: ");
	u8g_DrawStr(u8g, DMM_COLUMN2_POSX, DMM_ROW2_POSY, stringBuffer);
}