/*
 * UserControlBool.h
 *
 * Created: 28.09.2020 19:23:06
 *  Author: V17
 */ 

#ifndef USERCONTROLBOOL_H_
#define USERCONTROLBOOL_H_

#include "UserControlBase.h"

class UserControlBool : public UserControlBase<uint8_t>
{
private:
	
public:
	UserControlBool(uint8_t locx, uint8_t locy, uint8_t* controlValue) : UserControlBase(locx, locy, controlValue)
	{}
	
	void KeyInput(Keys_t key)
	{
		if(IsSelected && IsActive)
		{
			if(key == KEYLEFT || key == KEYRIGHT) { *_controlValue ^= true; }        // toggle control value
			else if(key == KEY0) { *_controlValue = false; }
			else if(key == KEY1) { *_controlValue = true; }
		}
	}
	
	void EncoderInput(EncoderDirection_t encDir)
	{
		if(IsSelected && IsActive)
		{
			if(encDir == ENCCLOCKWISE || encDir == ENCCOUNTERCLOCKWISE) { *_controlValue ^= true; }   // toggle control value
		}
	}
	
	void Draw(u8g_t *u8g)
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
};

#endif /* USERCONTROLBOOL_H_ */