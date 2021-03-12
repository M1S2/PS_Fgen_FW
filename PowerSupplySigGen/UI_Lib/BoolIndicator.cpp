/*
 * BoolIndicator.cpp
 *
 * Created: 12.03.2021 18:26:42
 *  Author: V17
 */ 

#include "BoolIndicator.h"

BoolIndicator::BoolIndicator(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, bool* controlValuePointer) : UIElement(locX, locY, width, height, UI_BOOLINDICATOR)
{
	_controlValuePointer = controlValuePointer;
}

void BoolIndicator::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (isFirstPage) { _controlValue = *_controlValuePointer; }

	if (Visible)
	{
		if (HasFocus)
		{
			
		}
		
		u8g_DrawStr(u8g, LocX, LocY, (_controlValue ? "true" : "false"));
	}
}