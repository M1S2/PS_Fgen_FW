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
#include "../Global/DevSettings.h"
#include <stdlib.h>
#include <stdbool.h>

#define CONTROLS_FONT_HEIGHT		8
#define CONTROLS_WIDTH				65

class UserControlBase
{
protected:
	uint8_t _locX;			// Location is upper left corner position
	uint8_t _locY;
	
public:
	bool IsSelected;
	bool IsActive;
	void (*OnValueChanged)();		// Function pointer for OnValueChanged event
		
	UserControlBase(uint8_t locx, uint8_t locy, void (*onValueChanged)())
	{
		_locX = locx;
		_locY = locy;
		OnValueChanged = onValueChanged;
	}
	
	virtual void KeyInput(Keys_t key);
	virtual void EncoderInput(EncoderDirection_t encDir);
	virtual void EncoderPBInput();
	
	virtual void Draw(u8g_t *u8g)
	{
		//u8g_SetFont(u8g, u8g_font_7x14r);		// 10 pixel height font
		u8g_SetFont(u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
		if(IsActive) { u8g_DrawBox(u8g, _locX, _locY, CONTROLS_WIDTH, CONTROLS_FONT_HEIGHT + 6); }
		else if(IsSelected) { u8g_DrawFrame(u8g, _locX, _locY, CONTROLS_WIDTH, CONTROLS_FONT_HEIGHT + 6); }
		else { u8g_DrawHLine(u8g, _locX, _locY + CONTROLS_FONT_HEIGHT + 6, CONTROLS_WIDTH); }
			
		if(IsActive) { u8g_SetDefaultBackgroundColor(u8g); }
		else { u8g_SetDefaultForegroundColor(u8g); }
	}
};

#endif /* USERCONTROLBASE_H_ */