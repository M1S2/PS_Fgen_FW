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
		if (HasFocus)
		{

		}
		u8g_DrawStr(u8g, LocX, LocY, (_controlValueDraw ? "true" : "false"));
	}
}

void BoolControl::ToggleValue()
{
	(*_controlValuePointer) = !(*_controlValuePointer);
}