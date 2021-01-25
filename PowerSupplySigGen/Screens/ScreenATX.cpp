/*
 * ScreenATX.cpp
 *
 * Created: 07.11.2020 13:23:55
 *  Author: V17
 */ 

#include "ScreenATX.h"

void ScreenATX::Draw(u8g_t* u8g, bool isFirstPage)
{
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW1_POSY + INDICATORS_FONT_HEIGHT, "3V3:");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW2_POSY + INDICATORS_FONT_HEIGHT, "5V:");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW3_POSY + INDICATORS_FONT_HEIGHT, "12V:");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW4_POSY + INDICATORS_FONT_HEIGHT, "-12V:");

	ScreenBase::Draw(u8g, isFirstPage);
}