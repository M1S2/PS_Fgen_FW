/*
 * UserControlBase.h
 *
 * Created: 28.09.2020 19:20:25
 *  Author: V17
 */ 


#ifndef USERCONTROLBASE_H_
#define USERCONTROLBASE_H_

#include "../KeyPad/KeyPad.h"
#include "../Encoder/Encoder.h"
#include "../GLCD/u8g.h"

#define CONTROLS_FONT_HEIGHT		10			// 10 pixel height font

template <class T>
class UserControlBase
{
protected:
	uint8_t _locX;			// Location is upper left corner position
	uint8_t _locY;
	char* _label;
	T* _controlValue;
	
public:
	bool IsSelected;
	bool IsActive;
	
	UserControlBase(uint8_t locx, uint8_t locy, char* label, T* controlValue)
	{
		_locX = locx;
		_locY = locy;
		_label = label;
		_controlValue = controlValue;
	}
	
	void KeyInput(Keys_t key);
	void EncoderInput(EncoderDirection_t encDir);
	void Draw(u8g_t *u8g)
	{
		//u8g_SetFont(u8g, u8g_font_4x6r);
		//u8g_DrawStr(u8g, _locX, _locY - 3, _label);
		
		u8g_SetFont(u8g, u8g_font_7x14r);		// 10 pixel height font
		if(IsActive) { u8g_DrawBox(u8g, _locX, _locY, 65, CONTROLS_FONT_HEIGHT + 6); u8g_SetDefaultBackgroundColor(u8g); }
		else if(IsSelected) { u8g_DrawFrame(u8g, _locX, _locY, 65, CONTROLS_FONT_HEIGHT + 6); }
	}
};

#endif /* USERCONTROLBASE_H_ */