/*
 * UserControlNumeric.h
 *
 * Created: 09.10.2020 19:02:40
 *  Author: V17
 */ 


#ifndef USERCONTROLNUMERIC_H_
#define USERCONTROLNUMERIC_H_

#include "UserControlBase.h"
#include <string.h>

template <class T>
class UserControlNumeric : public UserControlBase
{
    private:
        const char* _baseUnit;			// e.g. "V", "Hz", ...
        int8_t _valueStoreUnitPower;	// Is the control value stored e.g. in Millivolts (m = -3, k = 3, M = 6)
		T _minValue;
		T _maxValue;
		int8_t _currentDigitPosition;	// 2=left digit, 1, 0, -1, -2, -3=right digit
		int8_t _unitPrefixPower;		// Current display prefix power (m = -3, k = 3, M = 6)
		float _displayValue;			// Value that is displayed by Draw(). E.g. 999.760 if control value is 999760 Hz.
		T* _controlValue;

		T coerceValue(T value);		
		void calculateDisplayValue();		
		void recalculateControlValue();
		uint8_t extractDigit(float number, int8_t position);
		
    public:
        UserControlNumeric(uint8_t locx, uint8_t locy, T* controlValue, const char* baseUnit, int8_t valueStoreUnitPower, T minValue, T maxValue, void (*onValueChanged)());        
        void KeyInput(Keys_t key);
	    void EncoderInput(EncoderDirection_t encDir);		
		void EncoderPBInput();		
        void Draw(u8g_t *u8g, bool isFirstPage);
};


#endif /* USERCONTROLNUMERIC_H_ */