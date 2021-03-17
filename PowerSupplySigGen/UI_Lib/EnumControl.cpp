/*
 * EnumControl.cpp
 *
 * Created: 15.03.2021 19:19:15
 *  Author: V17
 */ 

#include "EnumControl.h"

template <class T>
EnumControl<T>::EnumControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, T* controlValuePointer, const char** enumNames, uint8_t numEnumValues) : UIElement(locX, locY, width, height, UI_ENUMCONTROL)
{
	_enumNames = enumNames;
	_numEnumValues = numEnumValues;
	_controlValuePointer = controlValuePointer;
}

template <class T>
void EnumControl<T>::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (isFirstPage) { _controlValueDraw = *_controlValuePointer; }

	if (Visible)
	{
		if (IsEditMode)
		{
			u8g_DrawBox(u8g, LocX, LocY, Width, Height);
			u8g_SetDefaultBackgroundColor(u8g);
		}	
		u8g_DrawHLine(u8g, LocX, LocY + Height, Width);	
		u8g_DrawStr(u8g, LocX, LocY, _enumNames[_controlValueDraw]);
		if(IsEditMode) { u8g_SetDefaultForegroundColor(u8g); }
	}
}

template <class T>
bool EnumControl<T>::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			return PreviousValue();
		case KEYDOWN:
			return NextValue();
		case KEYOK:
			ToggleEditMode();
			return true;
		default:
			return false;
	}
}

template <class T>
bool EnumControl<T>::PreviousValue()
{
	if (IsEditMode)
	{
		if (*_controlValuePointer > 0) { (*((int*)_controlValuePointer))--; }
		else if(*_controlValuePointer == 0) { (*_controlValuePointer) = (T)(_numEnumValues - 1); }
		return true;
	}
	return false;
}

template <class T>
bool EnumControl<T>::NextValue()
{
	if (IsEditMode)
	{
		if(*_controlValuePointer < (_numEnumValues - 1)) { (*((int*)_controlValuePointer))++; }
		else if(*_controlValuePointer >= (_numEnumValues - 1)) { (*_controlValuePointer) = (T)0; }
		return true;
	}
	return false;
}

template <class T>
void EnumControl<T>::ToggleEditMode()
{
	IsEditMode = !IsEditMode;
}