/*
 * screen_draw_TabDMM.cpp
 *
 * Created: 20.09.2020 12:33:46
 *  Author: V17
 */ 

#include "screen_draw_TabDMM.h"

/*Draw DMM tab with voltage info texts*/
void Screen_DrawTabDMM(u8g_t *u8g, DevStatus_t devStatusDraw)
{
	char stringBuffer[10];
	
	u8g_SetDefaultForegroundColor(u8g);
	u8g_SetFont(u8g, u8g_font_7x14r);	// 10 pixel height font
	
	/*Draw DMM1 info text*/
	dtostrf(devStatusDraw.DMM1, 6, 3, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, DMM_COLUMN1_POSX, DMM_ROW1_POSY, "DMM1: ");
	u8g_DrawStr(u8g, DMM_COLUMN2_POSX, DMM_ROW1_POSY, stringBuffer);
	
	/*Draw DMM2 info text*/
	dtostrf(devStatusDraw.DMM2, 6, 3, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, DMM_COLUMN1_POSX, DMM_ROW2_POSY, "DMM2: ");
	u8g_DrawStr(u8g, DMM_COLUMN2_POSX, DMM_ROW2_POSY, stringBuffer);
}