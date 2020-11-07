/*
 * ScreenBase.h
 *
 * Created: 05.11.2020 18:47:03
 *  Author: V17
 */ 


#ifndef SCREENBASE_H_
#define SCREENBASE_H_

#include "../USART/USART.h"
#include "../GLCD/u8g.h"
#include "../Global/DevStatus.h"
#include <stdlib.h>

#define SCREEN_TAB_WIDTH		32
#define SCREEN_TAB_HEIGHT		12
#define SCREEN_TAB_MARGIN		1
#define SCREEN_TAB_FONT_HEIGHT	8

class ScreenBase
{
	public:
		char* TabName;
	
		ScreenBase(char* tabName) : TabName(tabName)
		{}
		
		virtual void Draw(u8g_t* u8g, DevStatus_t devStatusDraw) = 0;
};

#endif /* SCREENBASE_H_ */