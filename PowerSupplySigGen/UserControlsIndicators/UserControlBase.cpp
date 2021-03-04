/*
 * UserControlBase.cpp
 *
 * Created: 28.01.2021 18:54:52
 *  Author: V17
 */ 

#include "UserControlBase.h"
#include "../Screens/Icons.h"

void UserControlBase::Draw(u8g_t *u8g, bool isFirstPage)
{
	u8g_SetFont(u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
	if(IsActive) 
	{ 
		u8g_DrawBox(u8g, _locX + CONTROLS_ICON_WIDTH, _locY, CONTROLS_WIDTH, CONTROLS_FONT_HEIGHT + 6); 
	}
	else if(IsSelected) 
	{ 
		u8g_DrawFrame(u8g, _locX + CONTROLS_ICON_WIDTH, _locY, CONTROLS_WIDTH, CONTROLS_FONT_HEIGHT + 6); 
	}
	else 
	{ 
		u8g_DrawHLine(u8g, _locX + CONTROLS_ICON_WIDTH, _locY + CONTROLS_FONT_HEIGHT + 6, CONTROLS_WIDTH); 
	}
	
	if(_iconWidth > 0 && _iconHeight > 0 && _iconBits != NULL)
	{
		u8g_DrawXBMP(u8g, _locX, _locY, _iconWidth, _iconHeight, _iconBits);
	}
	
	if(IsActive) 
	{ 
		u8g_SetDefaultBackgroundColor(u8g); 
	}
	else 
	{ 
		u8g_SetDefaultForegroundColor(u8g); 
	}
}