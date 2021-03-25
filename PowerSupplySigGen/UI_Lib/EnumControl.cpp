/*
 * EnumControl.cpp
 *
 * Created: 15.03.2021 19:19:15
 *  Author: V17
 */ 

#include "EnumControl.h"

template <class T>
EnumControl<T>::EnumControl(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, T* valuePointer, const char** enumNames, uint8_t numEnumValues) : EnumIndicator<T>(locX, locY, width, height, valuePointer, enumNames, numEnumValues)
{
	this->Type = UI_ENUMINDICATOR;
}

template <class T>
void EnumControl<T>::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (this->Visible)
	{
		if (IsEditMode)
		{
			u8g_DrawBox(u8g, this->LocX, this->LocY, this->Width, this->Height);
			u8g_SetDefaultBackgroundColor(u8g);
		}	
		else { u8g_DrawHLine(u8g, this->LocX, this->LocY + this->Height, this->Width); }
				
		EnumIndicator<T>::Draw(u8g, isFirstPage);
		
		if(IsEditMode) { u8g_SetDefaultForegroundColor(u8g); }
	}
}

template <class T>
bool EnumControl<T>::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			return NextValue();
		case KEYDOWN:
			return PreviousValue();
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
		if (*this->_valuePointer > 0) { (*((int*)this->_valuePointer))--; }
		else if(*this->_valuePointer == 0) { (*this->_valuePointer) = (T)(this->_numEnumValues - 1); }
		return true;
	}
	return false;
}

template <class T>
bool EnumControl<T>::NextValue()
{
	if (IsEditMode)
	{
		if(*this->_valuePointer < (this->_numEnumValues - 1)) { (*((int*)this->_valuePointer))++; }
		else if(*this->_valuePointer >= (this->_numEnumValues - 1)) { (*this->_valuePointer) = (T)0; }
		return true;
	}
	return false;
}

template <class T>
void EnumControl<T>::ToggleEditMode()
{
	IsEditMode = !IsEditMode;
}