/*
 * UserControlBase.cpp
 *
 * Created: 28.01.2021 18:54:52
 *  Author: V17
 */ 

#include "UserControlBase.h"

void UserControlBase::Draw(u8g_t *u8g, bool isFirstPage)
{
	u8g_SetFont(u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
	if(IsActive) 
	{ 
		u8g_DrawBox(u8g, _locX, _locY, CONTROLS_WIDTH, CONTROLS_FONT_HEIGHT + 6); 
	}
	else if(IsSelected) 
	{ 
		u8g_DrawFrame(u8g, _locX, _locY, CONTROLS_WIDTH, CONTROLS_FONT_HEIGHT + 6); 
	}
	else 
	{ 
		u8g_DrawHLine(u8g, _locX, _locY + CONTROLS_FONT_HEIGHT + 6, CONTROLS_WIDTH); 
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