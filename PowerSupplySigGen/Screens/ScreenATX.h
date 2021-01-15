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
		ScreenATX() : ScreenBase("ATX"),
			_indATX3V3(ATX_COLUMN2_POSX, ATX_ROW1_POSY, &DevStatus.ATX_3V3, "V", 0, 20, 100, 10),
			_indATX5V(ATX_COLUMN2_POSX, ATX_ROW2_POSY, &DevStatus.ATX_5V, "V", 0, 20, 100, 10),
			_indATX12V(ATX_COLUMN2_POSX, ATX_ROW3_POSY, &DevStatus.ATX_12V, "V", 0, 20, 100, 10),
			_indATX12VNEG(ATX_COLUMN2_POSX, ATX_ROW4_POSY, &DevStatus.ATX_12V_NEG, "V", -20, 0, 100, 10)
		{
			_userIndicators[0] = &_indATX3V3;
			_userIndicators[1] = &_indATX5V;
			_userIndicators[2] = &_indATX12V;
			_userIndicators[3] = &_indATX12VNEG;
			_numUserIndicators = 4;
		}

		virtual void Draw(u8g_t* u8g, DevStatus_t devStatusDraw);
};

#endif /* SCREENATX_H_ */