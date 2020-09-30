/*
 * screen_draw_TabATX.cpp
 *
 * Created: 19.09.2020 19:09:41
 *  Author: V17
 */ 

#include "screen_draw_TabATX.h"

/*Draw ATX tab with voltage info texts*/
void Screen_DrawTabATX(u8g_t *u8g, DevStatus_t devStatusDraw)
{
	char stringBuffer[10];
	
	u8g_SetDefaultForegroundColor(u8g);
	u8g_SetFont(u8g, u8g_font_7x14r);	// 10 pixel height font
	
	/*Draw +3V3 info text*/
	dtostrf(devStatusDraw.ATX_3V3_mV / 1000.0f, 5, 2, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW1_POSY, "+3V3: ");
	u8g_DrawStr(u8g, ATX_COLUMN2_POSX, ATX_ROW1_POSY, stringBuffer);
	
	/*Draw +5V info text*/
	dtostrf(devStatusDraw.ATX_5V_mV / 1000.0f, 6, 2, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, ATX_COLUMN3_POSX, ATX_ROW1_POSY, "+5V: ");
	u8g_DrawStr(u8g, ATX_COLUMN4_POSX, ATX_ROW1_POSY, stringBuffer);
	
	/*Draw +12V info text*/
	dtostrf(devStatusDraw.ATX_12V_mV / 1000.0f, 5, 2, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW2_POSY, "+12V: ");
	u8g_DrawStr(u8g, ATX_COLUMN2_POSX, ATX_ROW2_POSY, stringBuffer);
	
	/*Draw -12V info text*/
	dtostrf(devStatusDraw.ATX_12V_NEG_mV / -1000.0f, 6, 2, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, ATX_COLUMN3_POSX, ATX_ROW2_POSY, "-12V: ");
	u8g_DrawStr(u8g, ATX_COLUMN4_POSX, ATX_ROW2_POSY, stringBuffer);
}