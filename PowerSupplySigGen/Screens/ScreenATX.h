/*
 * ScreenATX.h
 *
 * Created: 07.11.2020 13:24:04
 *  Author: V17
 */ 


#ifndef SCREENATX_H_
#define SCREENATX_H_

#include "ScreenBase.h"

#define ATX_COLUMN1_POSX		SCREEN_TAB_WIDTH + 10
#define ATX_COLUMN2_POSX		ATX_COLUMN1_POSX + 35
#define ATX_COLUMN3_POSX		ATX_COLUMN2_POSX + 70
#define ATX_COLUMN4_POSX		ATX_COLUMN3_POSX + 35
#define ATX_ROW1_POSY			u8g_GetHeight(u8g) / 2 - 7
#define ATX_ROW2_POSY			u8g_GetHeight(u8g) / 2 + 10 + 7

class ScreenATX : public ScreenBase
{
	public:
	ScreenATX() : ScreenBase("ATX")
	{}

	virtual void Draw(u8g_t* u8g, DevStatus_t devStatusDraw);
};

#endif /* SCREENATX_H_ */