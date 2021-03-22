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

template <class T>
void NumericIndicator<T>::calculateDisplayValue()
{
	if (_lastValueDraw == _valueDraw) { return; }
	_lastValueDraw = _valueDraw;

	_displayValue = (float)_valueDraw;
	_unitPrefixPower = 0;

	if (_displayValue == 0)
	{
		/* Nothing to do here. Values are set above. */
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


template <class T>
NumericIndicator<T>::NumericIndicator(unsigned char locX, unsigned char locY, unsigned char width, unsigned char height, T* valuePointer, const char* baseUnit, unsigned char numNonFractionalDigits, unsigned char numFractionalDigits) : UIElement(locX, locY, width, height, UI_NUMERICINDICATOR)
{
	_valuePointer = valuePointer;
	_baseUnit = baseUnit;
	_unitPrefix = "";
	_numNonFractionalDigits = numNonFractionalDigits;
	_numFractionalDigits = numFractionalDigits;
}

template <class T>
void NumericIndicator<T>::Draw(u8g_t *u8g, bool isFirstPage)
{
	if (Visible)
	{
		if (isFirstPage) { _valueDraw = *_valuePointer; }

		calculateDisplayValue();

		//https://stackoverflow.com/questions/5932214/printf-string-variable-length-item
		char formatStringBuffer[10];
		sprintf(formatStringBuffer, "%%0%d.%df%s%s", _numNonFractionalDigits + _numFractionalDigits + 1 + (_numFractionalDigits > 0 ? 1 : 0), _numFractionalDigits, _unitPrefix, _baseUnit);	// if _numFractionalDigits is 0, no decimal point is used (one character less)

		char stringBufferLen = _numFractionalDigits + _numNonFractionalDigits + 1 + strlen(_unitPrefix) + strlen(_baseUnit) + 1;
		char stringBuffer[stringBufferLen];
		sprintf(stringBuffer, formatStringBuffer, fabs(_displayValue));
		
		if (_displayValue < 0) { stringBuffer[0] = '-'; }
		else { stringBuffer[0] = ' '; }

		u8g_DrawStr(u8g, LocX, LocY, stringBuffer);
	}
}