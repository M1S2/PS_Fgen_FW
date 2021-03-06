/*
 * UserControlEnum.h
 *
 * Created: 30.09.2020 21:05:03
 *  Author: V17
 */ 

#ifndef USERCONTROLENUM_H_
#define USERCONTROLENUM_H_

#include "UserControlBase.h"

template <class T>
class UserControlEnum : public UserControlBase
{
private:
	const char** _enumNames;
	uint8_t _numEnumValues;
	T* _controlValue;
	T _controlValueDraw;				// This variable is updated from the _controlValue on each draw of the first page.
	
public:
	UserControlEnum(uint8_t locx, uint8_t locy, T* controlValue, const char** enumNames, uint8_t numEnumValues, void* valueChangedContext, void (*onValueChanged)(void*), const u8g_pgm_uint8_t* iconBits = NULL);
	void KeyInput(Keys_t key);
	void EncoderInput(EncoderDirection_t encDir);	
	void EncoderPBInput();	
	void Draw(u8g_t *u8g, bool isFirstPage);
};

#endif /* USERCONTROLENUM_H_ */