/*
 * NumericIndicator.cpp
 *
 * Created: 22.03.2021 19:08:47
 *  Author: V17
 */ 

#include "NumericIndicator.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <stdlib.h>

template <class T>
void NumericIndicator<T>::calculateDisplayValue()
{
	_displayValue = (float)_valueDraw;
	_unitPrefixPower = 0;

	if (fabs(_displayValue) < pow(10, -_numFractionalDigits))
	{
		_displayValue = 0;
		/* _unitPrefixPower is set above. */
	}
	else if (fabs(_displayValue) >= 1000)
	{
		while (fabs(_displayValue) >= 1000)
		{
			_unitPrefixPower += 3;
			_displayValue /= 1000;
		}
	}
	else if (fabs(_displayValue) < 1)
	{
		while (fabs(_displayValue) < 1)
		{
			_unitPrefixPower -= 3;
			_displayValue *= 1000;
		}
	}

	_unitPrefix = (_unitPrefixPower == -3 ? "m" : (_unitPrefixPower == 0 ? "" : (_unitPrefixPower == 3 ? "k" : (_unitPrefixPower == 6 ? "M" : ""))));
}

// https://stackoverflow.com/questions/1489830/efficient-way-to-determine-number-of-digits-in-an-integer
template <class T>
int NumericIndicator<T>::numNonFractionalDigits(T number)
{
	int digits = 0;
	//if (number < 0) digits = 1; // remove this line if '-' counts as a digit
	while (fabs(number) >= 1)
	{
		number /= 10;
		digits++;
	}
	return digits;
}


template <class T>
NumericIndicator<T>::NumericIndicator(unsigned char locX, unsigned char locY, T* valuePointer, const char* baseUnit, T maxValue, unsigned char numFractionalDigits) : UIElement(locX, locY, UI_INDICATOR)
{
	_valuePointer = valuePointer;
	_baseUnit = baseUnit;
	_unitPrefix = "";
	_maxValue = maxValue;
	_numFractionalDigits = numFractionalDigits;
	_numDigits = _numFractionalDigits + numNonFractionalDigits(maxValue);
	_firstDraw = true;
}

template <class T>
void NumericIndicator<T>::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (Visible)
	{
		if (isFirstPage)
		{
			_valueDraw = *_valuePointer;
			if (_lastValueDraw != _valueDraw || _firstDraw)
			{
				_lastValueDraw = _valueDraw;
				calculateDisplayValue();

				//https://stackoverflow.com/questions/5932214/printf-string-variable-length-item
				char formatStringBuffer[10];
				sprintf(formatStringBuffer, "%%0%d.%df%s%s", _numDigits + (_numFractionalDigits > 0 ? 1 : 0), _numFractionalDigits + _unitPrefixPower, _unitPrefix, _baseUnit);       // if _numFractionalDigits is 0, no decimal point is used (one character less)

				sprintf(_stringDrawBuffer, formatStringBuffer, fabs(_displayValue));
			}
		}

		u8g_DrawStr(u8g, LocX + 5, LocY, _stringDrawBuffer);				// Draw value without minus sign
		if (_displayValue < 0) { u8g_DrawStr(u8g, LocX, LocY, "-"); }		// Draw minus sign
	}
}
