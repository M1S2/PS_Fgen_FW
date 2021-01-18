/*
 * ScreenDMM.cpp
 *
 * Created: 07.11.2020 13:15:21
 *  Author: V17
 */ 

#include "ScreenDMM.h"

void ScreenDMM::Draw(u8g_t* u8g)
{	
	u8g_DrawStr(u8g, DMM_COLUMN1_POSX, DMM_ROW1_POSY + INDICATORS_FONT_HEIGHT, "#1:");
	u8g_DrawStr(u8g, DMM_COLUMN1_POSX, DMM_ROW2_POSY + INDICATORS_FONT_HEIGHT, "#2:");

	ScreenBase::Draw(u8g);
}