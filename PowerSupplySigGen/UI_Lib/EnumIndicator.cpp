/*
 * EnumIndicator.cpp
 *
 * Created: 15.03.2021 19:42:23
 *  Author: V17
 */ 

#include "EnumIndicator.h"

template <class T>
EnumIndicator<T>::EnumIndicator(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, T* valuePointer, const char** enumNames, uint8_t numEnumValues) : UIElement(locX, locY, width, height, UI_INDICATOR)
{
	_enumNames = enumNames;
	_numEnumValues = numEnumValues;
	_valuePointer = valuePointer;
}

template <class T>
void EnumIndicator<T>::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (Visible)
	{
		if (isFirstPage) { _valueDraw = *_valuePointer; }
			
		u8g_DrawStr(u8g, LocX, LocY, _enumNames[_valueDraw]);
	}
}