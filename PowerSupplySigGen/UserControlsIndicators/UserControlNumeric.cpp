/*
 * UserControlNumeric.cpp
 *
 * Created: 28.01.2021 19:08:33
 *  Author: V17
 */ 

#include "UserControlNumeric.h"
#include "../Device.h"

template <class T>
T UserControlNumeric<T>::coerceValue(T value)
{
	if(value > _maxValue) { value = _maxValue; }
	else if(value < _minValue) { value = _minValue; }
	return value;
}

template <class T>
void UserControlNumeric<T>::calculateDisplayValue()
{
	_displayValue = (float)*_controlValue;
	if(_displayValue != 0)
	{
		_unitPrefixPower = _valueStoreUnitPower;
		while(fabs(_displayValue) >= 1000)
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

template <class T>
void UserControlNumeric<T>::recalculateControlValue()
{
	*_controlValue = coerceValue(_displayValue * pow(10, _unitPrefixPower) * pow(10, (- _valueStoreUnitPower)));
}

// https://www.quora.com/How-can-you-mathematically-extract-a-single-digit-from-a-number
template <class T>
uint8_t UserControlNumeric<T>::extractDigit(float number, int8_t position)
{
	float divisor = pow(10, position);
	uint32_t truncated = uint32_t((fabs(number) / divisor) + 0.1f);		// +0.1f not really clean workaround. Is there some rounding problem?
	//DebugDisplayNum = (fabs(number) / divisor) + 0.1f;
	return truncated % 10;
}

//------------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
UserControlNumeric<T>::UserControlNumeric(uint8_t locx, uint8_t locy, T* controlValue, const char* baseUnit, int8_t valueStoreUnitPower, T minValue, T maxValue, void (*onValueChanged)()) : UserControlBase(locx, locy, onValueChanged)
{
        _baseUnit = baseUnit;
        _valueStoreUnitPower = valueStoreUnitPower;
        _minValue = minValue;
        _maxValue = maxValue;
        _currentDigitPosition = 0;
        _unitPrefixPower = 0;
		_controlValue = controlValue;
        calculateDisplayValue();
}
  
template <class T>     
void UserControlNumeric<T>::KeyInput(Keys_t key)
{
    if(this->IsActive && key != KEYNONE)
    {
        if(key == KEYLEFT)			// Move cursor left
		{
				if(_currentDigitPosition < 2) { _currentDigitPosition++; } 
		}		
        else if(key == KEYRIGHT)	// Move cursor right
		{
			if(_currentDigitPosition > -3) { _currentDigitPosition--; } 
		}	
        else if(key == KEYMILLI) { this->IsActive = false; _unitPrefixPower = -3; recalculateControlValue(); calculateDisplayValue(); OnValueChanged(); Device.DevSettingsNeedSaving = true; }
        else if(key == KEYKILO) { this->IsActive = false; _unitPrefixPower = 3; recalculateControlValue(); calculateDisplayValue(); OnValueChanged(); Device.DevSettingsNeedSaving = true; }
        else if(key == KEYMINUS) { _displayValue *= -1; }
		else if(key == KEYCOMMA) { _displayValue = _displayValue / pow(10, (_currentDigitPosition + 1)); /*coerceDisplayValue();*/ _currentDigitPosition = -1; }
		else
		{
			uint8_t keyNum = KeyPad_GetKeyNumInt(key);
			uint8_t digit = extractDigit(_displayValue, _currentDigitPosition);				
			float multiplicator = pow(10, _currentDigitPosition);
			_displayValue = _displayValue - (digit * multiplicator) + (keyNum * multiplicator);
			//coerceDisplayValue();
			if(_currentDigitPosition > -3) { _currentDigitPosition--; }	// Move cursor right
		}
    }
}

template <class T>
void UserControlNumeric<T>::EncoderInput(EncoderDirection_t encDir)
{
	if(this->IsActive && encDir != ENCNONE)
	{
		// !!! recalculateControlValue on Encoder Button press
				
		//T value = *_controlValue;
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
		//*_controlValue = coerceValue(value);
		//calculateDisplayValue();
		//coerceDisplayValue();
	}
}

template <class T>		
void UserControlNumeric<T>::EncoderPBInput()
{
	recalculateControlValue();
	calculateDisplayValue();
	OnValueChanged();		
	Device.DevSettingsNeedSaving = true;
}
		
template <class T>
void UserControlNumeric<T>::Draw(u8g_t *u8g, bool isFirstPage)
{
	UserControlBase::Draw(u8g, isFirstPage);

	if(!this->IsActive)
	{
		calculateDisplayValue();
	}
		 	
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
			
	u8g_uint_t cursorXpos = this->_locX + (-_currentDigitPosition + 3) * 6 + (_currentDigitPosition < 0 ? 3 : 0);
	u8g_DrawHLine(u8g, cursorXpos, this->_locY + CONTROLS_FONT_HEIGHT + 4, 5);
}