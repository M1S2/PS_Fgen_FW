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
		if (HasFocus)
		{

		}		
		u8g_DrawStr(u8g, LocX, LocY, _enumNames[_controlValueDraw]);
	}
}

template <class T>
bool EnumControl<T>::PreviousValue()
{
	if (IsEditMode)
	{
		if (*_controlValuePointer > 0) { (*_controlValuePointer)--; }
		else if(*_controlValuePointer == 0) { (*_controlValuePointer) = _numEnumValues - 1; }
	}
}

template <class T>
bool EnumControl<T>::NextValue()
{
	if (IsEditMode)
	{
		if(*_controlValuePointer < (_numEnumValues - 1)) { (*_controlValuePointer)++; }
		else if(*_controlValuePointer >= (_numEnumValues - 1)) { (*_controlValuePointer) = 0; }
	}
}

template <class T>
void EnumControl<T>::ToggleEditMode()
{
	IsEditMode = !IsEditMode;
}