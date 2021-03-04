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
#include "../libraries/u8glib/u8g.h"
#include <stdlib.h>
#include <stdbool.h>

#include "../Configuration.h"

class UserControlBase
{
protected:
	uint8_t _locX;			// Location is upper left corner position
	uint8_t _locY;
	uint8_t _iconWidth;
	uint8_t _iconHeight;
	const u8g_pgm_uint8_t* _iconBits;
	
public:
	bool IsSelected;
	bool IsActive;
	void* ValueChangedContext;					// Context pointer that is returned with the OnValueChanged function pointer
	void (*OnValueChanged)(void* context);		// Function pointer for OnValueChanged event
		
	UserControlBase(uint8_t locx, uint8_t locy, void* valueChangedContext, void (*onValueChanged)(void*), uint8_t iconWidth = 0, uint8_t iconHeight = 0, const u8g_pgm_uint8_t* iconBits = NULL)
	{
		_locX = locx;
		_locY = locy;
		_iconWidth = iconWidth;
		_iconHeight = iconHeight;
		_iconBits = iconBits;	
		ValueChangedContext = valueChangedContext;
		OnValueChanged = onValueChanged;
	}
	
	virtual void KeyInput(Keys_t key);
	virtual void EncoderInput(EncoderDirection_t encDir);
	virtual void EncoderPBInput();
	virtual void Draw(u8g_t *u8g, bool isFirstPage);
};

#endif /* USERCONTROLBASE_H_ */