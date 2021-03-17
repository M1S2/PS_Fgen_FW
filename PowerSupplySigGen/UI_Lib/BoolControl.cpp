/*
 * BoolControl.cpp
 *
 * Created: 15.03.2021 19:12:22
 *  Author: V17
 */ 

#include "BoolControl.h"

BoolControl::BoolControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, bool* controlValuePointer) : UIElement(locX, locY, width, height, UI_BOOLCONTROL)
{
	_controlValuePointer = controlValuePointer;
}

void BoolControl::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (isFirstPage) { _controlValueDraw = *_controlValuePointer; }

	if (Visible)
	{
		u8g_DrawHLine(u8g, LocX, LocY + Height, Width);
		u8g_DrawStr(u8g, LocX, LocY, (_controlValueDraw ? "true" : "false"));
	}
}

bool BoolControl::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYOK:
			ToggleValue();
			return true;
		default:
			return false;
	}
}

void BoolControl::ToggleValue()
{
	(*_controlValuePointer) = !(*_controlValuePointer);
}