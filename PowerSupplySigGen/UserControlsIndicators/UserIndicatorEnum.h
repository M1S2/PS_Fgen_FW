/*
 * UserIndicatorEnum.h
 *
 * Created: 27.02.2021 20:02:57
 *  Author: V17
 */ 


#ifndef USERINDICATORENUM_H_
#define USERINDICATORENUM_H_

#include "UserIndicatorBase.h"

template <class T>
class UserIndicatorEnum : public UserIndicatorBase
{
	private:
		const char** _enumNames;
		uint8_t _numEnumValues;
		T* _controlValuePointer;
		T _controlValueDraw;				// This variable is updated from the _controlValue on each draw of the first page.
	
	public:
		UserIndicatorEnum(uint8_t locx, uint8_t locy, T* controlValuePointer, const char** enumNames, uint8_t numEnumValues);
		void Draw(u8g_t *u8g, bool isFirstPage);
};



#endif /* USERINDICATORENUM_H_ */