/*
 * UserControlBool.cpp
 *
 * Created: 28.09.2020 19:23:16
 *  Author: V17
 */ 

#include "UserControlBool.h"

void UserControlBool::KeyInput(Keys_t key)
{
	if(IsActive)
	{
		if(key == KEYLEFT || key == KEYRIGHT) { *_controlValue ^= true; }        // toggle control value
		else if(key == KEY0) { *_controlValue = false; }
		else if(key == KEY1) { *_controlValue = true; }
	}
}

void UserControlBool::EncoderInput(EncoderDirection_t encDir)
{
	if(IsActive)
	{
		if(encDir == ENCCLOCKWISE || encDir == ENCCOUNTERCLOCKWISE) { *_controlValue ^= true; }   // toggle control value
	}
}

void UserControlBool::Draw(u8g_t *u8g)
{
	UserControlBase::Draw(u8g);
	if(*_controlValue)
	{
		u8g_DrawStr(u8g, _locX + 3, _locY + 3 + CONTROLS_FONT_HEIGHT, "ON");
	}
	else
	{
		u8g_DrawStr(u8g, _locX + 3, _locY + 3 + CONTROLS_FONT_HEIGHT, "OFF");
	}
}