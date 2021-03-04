/*
 * UserIndicatorBase.h
 *
 * Created: 15.01.2021 19:20:25
 *  Author: V17
 */ 


#ifndef USERINDICATORBASE_H_
#define USERINDICATORBASE_H_

#include "../libraries/u8glib/u8g.h"
#include <stdlib.h>
#include <stdbool.h>

#include "../Configuration.h"

class UserIndicatorBase
{
protected:
	uint8_t _locX;			// Location is upper left corner position
	uint8_t _locY;
	
public:

	UserIndicatorBase(uint8_t locx, uint8_t locy);
	virtual void Draw(u8g_t *u8g, bool isFirstPage);
};

#endif /* USERINDICATORBASE_H_ */