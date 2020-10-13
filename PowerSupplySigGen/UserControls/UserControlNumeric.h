/*
 * UserControlNumeric.h
 *
 * Created: 09.10.2020 19:02:40
 *  Author: V17
 */ 


#ifndef USERCONTROLNUMERIC_H_
#define USERCONTROLNUMERIC_H_

#include "UserControlBase.h"

template <class T>
class UserControlNumeric : public UserControlBase<T>
{
    private:
        char* _baseUnit;				// e.g. "V", "Hz", ...
        int8_t _valueStoreUnitPower;	// Is the control value stored e.g. in Millivolts (m = -3, k = 3, M = 6)
		T _minValue;
		T _maxValue;
		int8_t _currentDigitPosition;	// 2=left digit, 1, 0, -1, -2, -3=right digit
		int8_t _unitPrefixPower;		// Current display prefix power (m = -3, k = 3, M = 6)
		float _displayValue;			// Value that is displayed by Draw(). E.g. 999.760 if control value is 999760 Hz.

		T coerceValue(T value)
		{
        	if(value > _maxValue) { value = _maxValue; }
        	else if(value < _minValue) { value = _minValue; }
        	return value;
		}
		
		void calculateDisplayValue()
		{
        	_displayValue = (float)*this->_controlValue;
			if(_displayValue != 0)
			{
        		_unitPrefixPower = _valueStoreUnitPower;
        		while(abs(_displayValue) >= 1000)
        		{
        			_unitPrefixPower += 3;
        			_displayValue /= 1000;
        		}
			}
			else
			{
				_unitPrefixPower = 0;
			}
		}
		
		void recalculateControlValue()
		{
		    *this->_controlValue = coerceValue(_displayValue * pow(10, _unitPrefixPower) * pow(10, (- _valueStoreUnitPower)));
		}
		
		// https://www.quora.com/How-can-you-mathematically-extract-a-single-digit-from-a-number
		uint8_t extractDigit(float number, int8_t position)
		{	
			float divisor = pow(10, position);
			uint32_t truncated = uint32_t(fabs(number) / divisor);
			return truncated % 10;
		}
		
    public:
        UserControlNumeric(uint8_t locx, uint8_t locy, T* controlValue, char* baseUnit, int8_t valueStoreUnitPower, T minValue, T maxValue) : UserControlBase<T>(locx, locy, controlValue)
        {
             _baseUnit = baseUnit;
             _valueStoreUnitPower = valueStoreUnitPower;
             _minValue = minValue;
             _maxValue = maxValue;
             _currentDigitPosition = 0;
             _unitPrefixPower = 0;
             calculateDisplayValue();
        }
        
        void KeyInput(Keys_t key)
        {
            if(this->IsActive && key != KEYNONE)
            {
                if(key == KEYLEFT && _currentDigitPosition < 2) { _currentDigitPosition++; }		// Move cursor left
        		else if(key == KEYRIGHT && _currentDigitPosition > -3) { _currentDigitPosition--; }	// Move cursor right
        		else if(key == KEYMILLI) { /*this->IsActive = false;*/ _unitPrefixPower = -3; recalculateControlValue(); calculateDisplayValue(); }
        		else if(key == KEYKILO) { /*this->IsActive = false;*/ _unitPrefixPower = 3; recalculateControlValue(); calculateDisplayValue(); }
        		else if(key == KEYMINUS) { _displayValue *= -1; }
				else if(key == KEYCOMMA) { _displayValue /= pow(10, (_currentDigitPosition + 1)); _currentDigitPosition = -1; }
				else
				{
					uint8_t keyNum = KeyPad_GetKeyNumInt(key);
					uint8_t digit = extractDigit(_displayValue, _currentDigitPosition);
					float multiplicator = pow(10, _currentDigitPosition);
					_displayValue = _displayValue - (digit * multiplicator) + (keyNum * multiplicator);
					if(_currentDigitPosition > -3) { _currentDigitPosition--; }	// Move cursor right
				}
            }
        }
	    void EncoderInput(EncoderDirection_t encDir)
		{
			if(this->IsActive && encDir != ENCNONE)
			{
				// !!! recalculateControlValue on Encoder Button press
				
				//T value = *this->_controlValue;
				//T deltaValue = (pow(10, _currentDigitPosition + _unitPrefixPower - _valueStoreUnitPower));
				float deltaValue = pow(10, _currentDigitPosition);
				
				if(encDir == ENCCLOCKWISE)
				{
					//value += deltaValue;
					_displayValue += deltaValue;
				}
				else if(encDir == ENCCOUNTERCLOCKWISE)
				{
					//value -= deltaValue;
					_displayValue -= deltaValue;
				}
				//*this->_controlValue = coerceValue(value);
				//calculateDisplayValue();
			}
		}
		
        void Draw(u8g_t *u8g)
		{
			UserControlBase<T>::Draw(u8g);
 
			char stringBufferLen = 8 + strlen(_baseUnit) + 1 + (_unitPrefixPower == 0 ? 0 : 1);
 			char stringBuffer[stringBufferLen];
			dtostrf(fabs(_displayValue), 8, 3, stringBuffer);
			strcat(stringBuffer, (_unitPrefixPower == -3 ? "m" : (_unitPrefixPower == 0 ? "" : (_unitPrefixPower == 3 ? "k" : (_unitPrefixPower == 6 ? "M" : "")))));
			strcat(stringBuffer, _baseUnit);
			
			for(int i = 1; i < stringBufferLen; i++)
			{
				if (stringBuffer[i] == ' ') { stringBuffer[i] = '0'; }
			}
			if(_displayValue < 0) { stringBuffer[0] = '-'; }
			u8g_DrawStr(u8g, this->_locX + 3 - (_displayValue < 0 ? 1 : 0), this->_locY + 3 + CONTROLS_FONT_HEIGHT, stringBuffer);
			
			u8g_SetDefaultBackgroundColor(u8g);
			u8g_uint_t cursorXpos = this->_locX + (-_currentDigitPosition + 3) * 6 + (_currentDigitPosition < 0 ? 3 : 0);
			u8g_DrawHLine(u8g, cursorXpos, this->_locY + CONTROLS_FONT_HEIGHT + 4, 5);
		}
};


#endif /* USERCONTROLNUMERIC_H_ */