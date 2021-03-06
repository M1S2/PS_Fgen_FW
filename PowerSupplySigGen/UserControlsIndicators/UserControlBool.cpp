/*
 * UserControlBool.cpp
 *
 * Created: 28.01.2021 19:00:10
 *  Author: V17
 */ 

#include "UserControlBool.h"

UserControlBool::UserControlBool(uint8_t locx, uint8_t locy, bool* controlValue, void* valueChangedContext, void (*onValueChanged)(void*), const u8g_pgm_uint8_t* iconBits) : UserControlBase(locx, locy, valueChangedContext, onValueChanged, iconBits)
{
	_controlValue = controlValue;
}

void UserControlBool::KeyInput(Keys_t key)
{
	/* Nothing to do here, because the value is toggled when the control gets activated. Afterwards the control is deactivated immediately. */
}
	
void UserControlBool::EncoderInput(EncoderDirection_t encDir)
{
	/* Nothing to do here, because the value is toggled when the control gets activated. Afterwards the control is deactivated immediately. */
}
	
void UserControlBool::EncoderPBInput()
{
	if(IsSelected && IsActive)
	{
		if(*_controlValue == true) { *_controlValue = false; }
		else { *_controlValue = true; }
		IsActive = false;
		OnValueChanged(ValueChangedContext);
	}
}
	
void UserControlBool::Draw(u8g_t *u8g, bool isFirstPage)
{
	UserControlBase::Draw(u8g, isFirstPage);

	if (isFirstPage) { _controlValueDraw = *_controlValue; }

	if(_controlValueDraw)
	{
		u8g_DrawStr(u8g, _locX + CONTROLS_ICON_SIZE + 4, _locY + 3 + CONTROLS_FONT_HEIGHT, "ON");
	}
	else
	{
		u8g_DrawStr(u8g, _locX + CONTROLS_ICON_SIZE + 4, _locY + 3 + CONTROLS_FONT_HEIGHT, "OFF");
	}
}