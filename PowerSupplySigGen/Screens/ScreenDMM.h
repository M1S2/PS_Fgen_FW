/*
 * ScreenDMM.h
 *
 * Created: 07.11.2020 13:15:27
 *  Author: V17
 */ 


#ifndef SCREENDMM_H_
#define SCREENDMM_H_

#include "ScreenBase.h"

#define DMM_COLUMN1_POSX		SCREEN_TAB_WIDTH + 10
#define DMM_COLUMN2_POSX		DMM_COLUMN1_POSX + 50
#define DMM_ROW1_POSY			u8g_GetHeight(u8g) / 2 - 7
#define DMM_ROW2_POSY			u8g_GetHeight(u8g) / 2 + 10 + 7

class ScreenDMM : public ScreenBase
{
	public:
		ScreenDMM() : ScreenBase("DMM")
		{}

		virtual void Draw(u8g_t* u8g, DevStatus_t devStatusDraw);
};

#endif /* SCREENDMM_H_ */