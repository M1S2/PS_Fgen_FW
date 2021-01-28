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
    T* _controlValuePointer;
	T _controlValue;				// This variable is updated from the _controlValuePointer on each draw of the first page.
	float _displayValue;			// Value that is displayed by Draw(). E.g. 999.760 if control value is 999760 Hz.
	const char* _unitPrefix;		// Current display prefix (m, k, M)	

	void calculateDisplayValue();
		
public:
	UserIndicatorNumeric(uint8_t locx, uint8_t locy, T* controlValuePointer, const char* baseUnit);        		
    void Draw(u8g_t *u8g, bool isFirstPage);
};


#endif /* USERINDICATORNUMERIC_H_ */