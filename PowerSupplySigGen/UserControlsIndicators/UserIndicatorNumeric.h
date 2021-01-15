/*
 * UserIndicatorNumeric.h
 *
 * Created: 15.01.2021 19:02:40
 *  Author: V17
 */ 


#ifndef USERINDICATORNUMERIC_H_
#define USERINDICATORNUMERIC_H_

#include "UserIndicatorBase.h"
#include <string.h>

template <class T>
class UserIndicatorNumeric : public UserIndicatorBase
{
private:
	T _lastControlValue;

protected:
    const char* _baseUnit;			// e.g. "V", "Hz", ...
    T* _controlValue;
	float _displayValue;			// Value that is displayed by Draw(). E.g. 999.760 if control value is 999760 Hz.
	const char* _unitPrefix;		// Current display prefix (m, k, M)	

	void calculateDisplayValue()
	{
		if (_lastControlValue == *_controlValue) { return; }
		_lastControlValue = *_controlValue;
		
		int8_t unitPrefixPower;		// Current display prefix power (m = -3, k = 3, M = 6)

		_displayValue = (float)*_controlValue;
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
		
public:
	UserIndicatorNumeric(uint8_t locx, uint8_t locy, T* controlValue, const char* baseUnit) : UserIndicatorBase(locx, locy)
    {
            _baseUnit = baseUnit;
            _controlValue = controlValue;
	}
        		
    void Draw(u8g_t *u8g)
	{
		UserIndicatorBase::Draw(u8g);
 		
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
};


#endif /* USERINDICATORNUMERIC_H_ */