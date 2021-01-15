/*
 * ScreenATX.cpp
 *
 * Created: 07.11.2020 13:23:55
 *  Author: V17
 */ 

#include "ScreenATX.h"

void ScreenATX::Draw(u8g_t* u8g, DevStatus_t devStatusDraw)
{
#if 0
	char stringBuffer[10];
	
	u8g_SetDefaultForegroundColor(u8g);
	u8g_SetFont(u8g, u8g_font_7x14r);	// 10 pixel height font
	
	/*Draw +3V3 info text*/
	dtostrf(devStatusDraw.ATX_3V3, 5, 2, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW1_POSY, "+3V3: ");
	u8g_DrawStr(u8g, ATX_COLUMN2_POSX, ATX_ROW1_POSY, stringBuffer);
	
	/*Draw +5V info text*/
	dtostrf(devStatusDraw.ATX_5V, 6, 2, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, ATX_COLUMN3_POSX, ATX_ROW1_POSY, "+5V: ");
	u8g_DrawStr(u8g, ATX_COLUMN4_POSX, ATX_ROW1_POSY, stringBuffer);
	
	/*Draw +12V info text*/
	dtostrf(devStatusDraw.ATX_12V, 5, 2, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW2_POSY, "+12V: ");
	u8g_DrawStr(u8g, ATX_COLUMN2_POSX, ATX_ROW2_POSY, stringBuffer);
	
	/*Draw -12V info text*/
	dtostrf(devStatusDraw.ATX_12V_NEG, 6, 2, stringBuffer);
	strcat(stringBuffer, " V");
	u8g_DrawStr(u8g, ATX_COLUMN3_POSX, ATX_ROW2_POSY, "-12V: ");
	u8g_DrawStr(u8g, ATX_COLUMN4_POSX, ATX_ROW2_POSY, stringBuffer);
#endif

	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW1_POSY + INDICATORS_FONT_HEIGHT, "3V3:");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW2_POSY + INDICATORS_FONT_HEIGHT, "5V:");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW3_POSY + INDICATORS_FONT_HEIGHT, "12V:");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW4_POSY + INDICATORS_FONT_HEIGHT, "-12V:");

	ScreenBase::Draw(u8g, devStatusDraw);
}