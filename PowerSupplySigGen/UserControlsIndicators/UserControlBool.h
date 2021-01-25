/*
 * UserControlBool.h
 *
 * Created: 28.09.2020 19:23:06
 *  Author: V17
 */ 

#ifndef USERCONTROLBOOL_H_
#define USERCONTROLBOOL_H_

#include "UserControlBase.h"

class UserControlBool : public UserControlBase
{
private:
	bool* _controlValue;
	bool _controlValueDraw;				// This variable is updated from the _controlValue on each draw of the first page.
	
public:
	
	UserControlBool(uint8_t locx, uint8_t locy, bool* controlValue, void (*onValueChanged)()) : UserControlBase(locx, locy, onValueChanged)
	{
		_controlValue = controlValue;
	}
	
	void KeyInput(Keys_t key)
	{
		/* Nothing to do here, because the value is toggled when the control gets activated. Afterwards the control is deactivated immediately. */
	}
	
	void EncoderInput(EncoderDirection_t encDir)
	{
		/* Nothing to do here, because the value is toggled when the control gets activated. Afterwards the control is deactivated immediately. */
	}
	
	void EncoderPBInput()
	{
		if(IsSelected && IsActive)
		{
			if(*_controlValue == true) { *_controlValue = false; }
			else { *_controlValue = true; }
			IsActive = false;
			OnValueChanged();
			DevSettingsNeedSaving = true;
		}
	}
	
	void Draw(u8g_t *u8g, bool isFirstPage)
	{
		UserControlBase::Draw(u8g, isFirstPage);

		if (isFirstPage) { _controlValueDraw = *_controlValue; }

		if(_controlValueDraw)
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