/*
 * ScreenATX.h
 *
 * Created: 07.11.2020 13:24:04
 *  Author: V17
 */ 


#ifndef SCREENATX_H_
#define SCREENATX_H_

#include "ScreenBase.h"
#include "../UserControlsIndicators/UserIndicatorProgressBar.h"

#define ATX_COLUMN1_POSX		SCREEN_TAB_WIDTH + 7
#define ATX_COLUMN2_POSX		ATX_COLUMN1_POSX + 35
#define ATX_ROW1_POSY			15
#define ATX_ROW2_POSY			ATX_ROW1_POSY + 12
#define ATX_ROW3_POSY			ATX_ROW2_POSY + 12
#define ATX_ROW4_POSY			ATX_ROW3_POSY + 12

class ScreenATX : public ScreenBase
{
	private:
		UserIndicatorProgressBar<float> _indATX3V3;
		UserIndicatorProgressBar<float> _indATX5V;
		UserIndicatorProgressBar<float> _indATX12V;
		UserIndicatorProgressBar<float> _indATX12VNEG;

	public:
		ScreenATX();
		virtual void Draw(u8g_t* u8g, bool isFirstPage);
};

#endif /* SCREENATX_H_ */