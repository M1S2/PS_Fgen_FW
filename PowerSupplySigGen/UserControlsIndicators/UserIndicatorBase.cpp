/*
 * UserIndicatorBase.cpp
 *
 * Created: 28.01.2021 19:25:11
 *  Author: V17
 */ 

#include "UserIndicatorBase.h"

UserIndicatorBase::UserIndicatorBase(uint8_t locx, uint8_t locy)
{
	_locX = locx;
	_locY = locy;
}
	
void UserIndicatorBase::Draw(u8g_t *u8g, bool isFirstPage)
{
	u8g_SetFont(u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
	u8g_SetDefaultForegroundColor(u8g);
}