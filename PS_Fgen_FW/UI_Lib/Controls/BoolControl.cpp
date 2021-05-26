/*
 * BoolControl.cpp
 *
 * Created: 15.03.2021 19:12:22
 *  Author: V17
 */ 

#include "BoolControl.h"

BoolControl::BoolControl(unsigned char locX, unsigned char locY, bool* valuePointer, void* controlContext, void(*onValueChanged)(void* controlContext)) : BoolIndicator(locX, locY, valuePointer)
{
	Type = UI_CONTROL;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
}

void BoolControl::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (Visible)
	{
		BoolIndicator::Draw(u8g, isFirstPage);
		u8g_DrawHLine(u8g, LocX, LocY + Height, Width);
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
	(*_valuePointer) = !(*_valuePointer);
	if (_onValueChanged != NULL) { _onValueChanged(_controlContext); }
}