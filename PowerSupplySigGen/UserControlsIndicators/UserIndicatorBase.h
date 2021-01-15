/*
 * UserIndicatorBase.h
 *
 * Created: 15.01.2021 19:20:25
 *  Author: V17
 */ 


#ifndef USERINDICATORBASE_H_
#define USERINDICATORBASE_H_

#include "../GLCD/u8g.h"
#include <stdlib.h>
#include <stdbool.h>

#include "../Configuration.h"

class UserIndicatorBase
{
protected:
	uint8_t _locX;			// Location is upper left corner position
	uint8_t _locY;
	
public:

	UserIndicatorBase(uint8_t locx, uint8_t locy)
	{
		_locX = locx;
		_locY = locy;
	}
	
	virtual void Draw(u8g_t *u8g)
	{
		u8g_SetFont(u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
		u8g_SetDefaultForegroundColor(u8g);
	}
};

#endif /* USERINDICATORBASE_H_ */