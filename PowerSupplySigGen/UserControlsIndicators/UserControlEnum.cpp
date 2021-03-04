/*
 * UserControlEnum.cpp
 *
 * Created: 28.01.2021 19:03:30
 *  Author: V17
 */ 

#include "UserControlEnum.h"

template <class T>
UserControlEnum<T>::UserControlEnum(uint8_t locx, uint8_t locy, T* controlValue, const char** enumNames, uint8_t numEnumValues, void* valueChangedContext, void (*onValueChanged)(void*), uint8_t iconWidth, uint8_t iconHeight, const u8g_pgm_uint8_t* iconBits) : UserControlBase(locx, locy, valueChangedContext, onValueChanged, iconWidth, iconHeight, iconBits)
{
	_enumNames = enumNames;
	_numEnumValues = numEnumValues;
	_controlValue = controlValue;
}

template <class T>
void UserControlEnum<T>::KeyInput(Keys_t key)
{
	if(this->IsSelected && this->IsActive)
	{
		uint8_t* value = (uint8_t*)_controlValue;
		
		if(key == KEYRIGHT) { EncoderInput(ENCCLOCKWISE); }
		else if(key == KEYLEFT) { EncoderInput(ENCCOUNTERCLOCKWISE); }
		else if(key == KEY0 && _numEnumValues > 0) { *value = 0; OnValueChanged(ValueChangedContext); }
		else if(key == KEY1 && _numEnumValues > 1) { *value = 1; OnValueChanged(ValueChangedContext); }
		else if(key == KEY2 && _numEnumValues > 2) { *value = 2; OnValueChanged(ValueChangedContext); }
		else if(key == KEY3 && _numEnumValues > 3) { *value = 3; OnValueChanged(ValueChangedContext); }
		else if(key == KEY4 && _numEnumValues > 4) { *value = 4; OnValueChanged(ValueChangedContext); }
		else if(key == KEY5 && _numEnumValues > 5) { *value = 5; OnValueChanged(ValueChangedContext); }
		else if(key == KEY6 && _numEnumValues > 6) { *value = 6; OnValueChanged(ValueChangedContext); }
		else if(key == KEY7 && _numEnumValues > 7) { *value = 7; OnValueChanged(ValueChangedContext); }
		else if(key == KEY8 && _numEnumValues > 8) { *value = 8; OnValueChanged(ValueChangedContext); }
		else if(key == KEY9 && _numEnumValues > 9) { *value = 9; OnValueChanged(ValueChangedContext); }
	}
}

template <class T>
void UserControlEnum<T>::EncoderInput(EncoderDirection_t encDir)
{
	if(this->IsSelected && this->IsActive)
	{
		uint8_t* value = (uint8_t*)_controlValue;
		
		if(encDir == ENCCLOCKWISE && *value < (_numEnumValues - 1)) { (*value)++; }
		else if(encDir == ENCCLOCKWISE && *value >= (_numEnumValues - 1)) { (*value) = 0; }
		else if (encDir == ENCCOUNTERCLOCKWISE && *value > 0) { (*value)--; }
		else if(encDir == ENCCOUNTERCLOCKWISE && *value == 0) { (*value) = _numEnumValues - 1; }
		
		OnValueChanged(ValueChangedContext);
	}
}

template <class T>
void UserControlEnum<T>::EncoderPBInput()
{
	/* Nothing to do here */
}

template <class T>
void UserControlEnum<T>::Draw(u8g_t *u8g, bool isFirstPage)
{
	UserControlBase::Draw(u8g, isFirstPage);
	if (isFirstPage) { _controlValueDraw = *_controlValue; }
	
	u8g_DrawStr(u8g, this->_locX + CONTROLS_ICON_WIDTH + 3, this->_locY + 3 + CONTROLS_FONT_HEIGHT, _enumNames[_controlValueDraw]);
}