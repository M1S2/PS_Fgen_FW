/*
 * UserIndicatorEnum.cpp
 *
 * Created: 27.02.2021 20:03:07
 *  Author: V17
 */ 

#include "UserIndicatorEnum.h"

template <class T>
UserIndicatorEnum<T>::UserIndicatorEnum(uint8_t locx, uint8_t locy, T* controlValuePointer, const char** enumNames, uint8_t numEnumValues) : UserIndicatorBase(locx, locy)
{
	_enumNames = enumNames;
	_numEnumValues = numEnumValues;
	_controlValuePointer = controlValuePointer;
}

template <class T>
void UserIndicatorEnum<T>::Draw(u8g_t *u8g, bool isFirstPage)
{
	UserIndicatorBase::Draw(u8g, isFirstPage);
	if (isFirstPage) { _controlValueDraw = *_controlValuePointer; }
	
	u8g_DrawStr(u8g, this->_locX + 3, this->_locY + 3 + CONTROLS_FONT_HEIGHT, _enumNames[_controlValueDraw]);
}