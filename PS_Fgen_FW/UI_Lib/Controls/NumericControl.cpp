/*
 * NumericControl.cpp
 * Created: 25.03.2021 20:52:48
 * Author: Markus Scheich
 */ 

#include "NumericControl.h"
#include <math.h>

template <class T, int stringBufferLength>
NumericControl<T, stringBufferLength>::NumericControl(unsigned char locX, unsigned char locY, T* valuePointer, const char* baseUnit, T minValue, T maxValue, int numFractionalDigits, void* controlContext, void(*onValueChanged)(void* controlContext)) : NumericIndicator<T, stringBufferLength>(locX, locY, valuePointer, baseUnit, maxValue, numFractionalDigits)
{
	this->Type = UI_CONTROL;
	IsEditMode = false;
	_currentDigitPosition = 0;
	_minValue = minValue;
	_controlContext = controlContext;
	_onValueChanged = onValueChanged;
}

template <class T, int stringBufferLength>
void NumericControl<T, stringBufferLength>::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (this->Visible)
	{
		if (IsEditMode)
		{
			u8g_DrawBox(u8g, this->LocX, this->LocY, this->Width, this->Height);
			u8g_SetDefaultBackgroundColor(u8g);
		}
		else { u8g_DrawHLine(u8g, this->LocX, this->LocY + this->Height, this->Width); }
		
		NumericIndicator<T, stringBufferLength>::Draw(u8g, isFirstPage);
		
		if(IsEditMode)
		{																								 
			uint8_t cursorDigitIndex = (-_currentDigitPosition + (this->_numDigits - this->_numFractionalDigits)) + (((this->_numFractionalDigits + this->_unitPrefixPower) == 0 && this->_numFractionalDigits != 0) ? 1 : 0);	// if (this->_numFractionalDigits + this->_unitPrefixPower) == 0,  no comma is available
			uint8_t cursorXpos = this->LocX + cursorDigitIndex * 6 + (_currentDigitPosition < this->_unitPrefixPower ? 3 : 0) - 1;																								// if (_currentDigitPosition < _unitPrefixPower) cursor is right of comma
			u8g_DrawHLine(u8g, cursorXpos, this->LocY + this->Height, 5);		// Draw cursor

			u8g_SetDefaultForegroundColor(u8g); 
		}
	}
}

template <class T, int stringBufferLength>
T NumericControl<T, stringBufferLength>::coerceValue(T value)
{
	if (value > this->_maxValue) { value = this->_maxValue; }
	else if (value < _minValue) { value = _minValue; }
	return value;
}

template <class T, int stringBufferLength>
unsigned char NumericControl<T, stringBufferLength>::extractDigit(float number, int8_t position)
{
	float divisor = pow(10, position);
	uint32_t truncated = uint32_t((fabs(number) / divisor) + 0.1f);		// +0.1f not really clean workaround. Is there some rounding problem?
	return truncated % 10;
}


template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYLEFT:
			return CursorLeft();
		case KEYRIGHT:
			return CursorRight();
		case KEYUP:
			return ValueUp();
		case KEYDOWN:
			return ValueDown();
		case KEYOK:
			ToggleEditMode();
			return true;
		case KEYKILO:
			return KeyKilo();
		case KEYMILLI:
			return KeyMilli();
		case KEYX1:
			return KeyX1();
		case KEYMINUS:
			return KeyMinus();
		case KEY0:
		case KEY1:
		case KEY2:
		case KEY3:
		case KEY4:
		case KEY5:
		case KEY6:
		case KEY7:
		case KEY8:
		case KEY9:
			return KeyNumeric(key);
		default:
			return false;
	}
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyKilo()
{
	if (IsEditMode)
	{
		float multiplicator = pow(10, 3 - this->_unitPrefixPower);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) * multiplicator);
		
		if (_onValueChanged != NULL && multiplicator != 1) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyMilli()
{
	if (IsEditMode)
	{
		float multiplicator = pow(10, -3 - this->_unitPrefixPower);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) * multiplicator);
		
		if (_onValueChanged != NULL && multiplicator != 1) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyX1()
{
	if (IsEditMode)
	{
		float multiplicator = pow(10, 0 - this->_unitPrefixPower);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) * multiplicator);
		
		if (_onValueChanged != NULL && multiplicator != 1) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyMinus()
{
	if (IsEditMode)
	{
		(*this->_valuePointer) = coerceValue(-(*this->_valuePointer));
		
		if (_onValueChanged != NULL && (*this->_valuePointer) != 0) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::KeyNumeric(Keys_t key)
{
	if (IsEditMode)
	{
		T oldValue = *this->_valuePointer;
		
		unsigned char keyNum = Keys_GetKeyNumInt(key);
		if(keyNum < 0 || keyNum > 9) { return false; }		// if the keyNum isn't in the range 0..9, the given key is no numeric key
			
		unsigned char digit = extractDigit(oldValue, _currentDigitPosition);
		float multiplicator = pow(10, _currentDigitPosition);
		(*this->_valuePointer) = coerceValue(oldValue - (digit * multiplicator) + (keyNum * multiplicator));
		if (_currentDigitPosition > -this->_numFractionalDigits) { _currentDigitPosition--; }	// Move cursor right

		if (_onValueChanged != NULL && oldValue != (*this->_valuePointer)) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::ValueUp()
{
	if (IsEditMode)
	{
		float deltaValue = pow(10, _currentDigitPosition);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) + deltaValue);
		
		if (_onValueChanged != NULL && deltaValue != 0) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::ValueDown()
{
	if (IsEditMode)
	{
		float deltaValue = pow(10, _currentDigitPosition);
		(*this->_valuePointer) = coerceValue((*this->_valuePointer) - deltaValue);
		
		if (_onValueChanged != NULL && deltaValue != 0) { _onValueChanged(_controlContext); }
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::CursorLeft()
{
	if (IsEditMode)
	{
		if (_currentDigitPosition < (this->_numDigits - this->_numFractionalDigits - 1))
		{
			_currentDigitPosition++;
		}
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
bool NumericControl<T, stringBufferLength>::CursorRight()
{
	if (IsEditMode)
	{
		if (_currentDigitPosition > -this->_numFractionalDigits)
		{
			_currentDigitPosition--;
		}
		return true;
	}
	return false;
}

template <class T, int stringBufferLength>
void NumericControl<T, stringBufferLength>::ToggleEditMode()
{
	IsEditMode = !IsEditMode;
}