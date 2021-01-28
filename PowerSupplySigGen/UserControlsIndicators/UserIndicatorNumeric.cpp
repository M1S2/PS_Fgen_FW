/*
 * UserIndicatorNumeric.cpp
 *
 * Created: 28.01.2021 19:26:39
 *  Author: V17
 */ 

#include "UserIndicatorNumeric.h"
#include <math.h>

template <class T>
void UserIndicatorNumeric<T>::calculateDisplayValue()
{
	if (_lastControlValue == _controlValue) { return; }
	_lastControlValue = _controlValue;
	
	int8_t unitPrefixPower;		// Current display prefix power (m = -3, k = 3, M = 6)

	_displayValue = (float)_controlValue;
	unitPrefixPower = 0;
	
	if(_displayValue == 0)
	{
		/* Nothing to do here. Values are set above. */
	}
	else if (fabs(_displayValue) >= 1000)
	{
		while (fabs(_displayValue) >= 1000)
		{
			unitPrefixPower += 3;
			_displayValue /= 1000;
		}
	}
	else if (fabs(_displayValue) < 1)
	{
		while (fabs(_displayValue) < 1)
		{
			unitPrefixPower -= 3;
			_displayValue *= 1000;
		}
	}

	_unitPrefix = (unitPrefixPower == -3 ? "m" : (unitPrefixPower == 0 ? "" : (unitPrefixPower == 3 ? "k" : (unitPrefixPower == 6 ? "M" : ""))));
}

//------------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
UserIndicatorNumeric<T>::UserIndicatorNumeric(uint8_t locx, uint8_t locy, T* controlValuePointer, const char* baseUnit) : UserIndicatorBase(locx, locy)
{
	_baseUnit = baseUnit;
	_controlValuePointer = controlValuePointer;
}

template <class T>
void UserIndicatorNumeric<T>::Draw(u8g_t *u8g, bool isFirstPage)
{
	UserIndicatorBase::Draw(u8g, isFirstPage);
	
	if (isFirstPage) { _controlValue = *_controlValuePointer; }

	calculateDisplayValue();
	
	char stringBufferLen = 8 + strlen(_baseUnit) + 1 + strlen(_unitPrefix);
	char stringBuffer[stringBufferLen];
	dtostrf(fabs(_displayValue), 8, 3, stringBuffer);
	strcat(stringBuffer, _unitPrefix);
	strcat(stringBuffer, _baseUnit);
	
	for(int i = 1; i < stringBufferLen; i++)
	{
		if (stringBuffer[i] == ' ') { stringBuffer[i] = '0'; }
	}
	if(_displayValue < 0) { stringBuffer[0] = '-'; }
	u8g_DrawStr(u8g, this->_locX - (_displayValue < 0 ? 1 : 0), this->_locY + INDICATORS_FONT_HEIGHT, stringBuffer);
}