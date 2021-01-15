/*
 * ScreenDMM.h
 *
 * Created: 07.11.2020 13:15:27
 *  Author: V17
 */ 


#ifndef SCREENDMM_H_
#define SCREENDMM_H_

#include "ScreenBase.h"
#include "../UserControlsIndicators/UserIndicatorProgressBar.h"

#define DMM_COLUMN1_POSX		SCREEN_TAB_WIDTH + 7
#define DMM_COLUMN2_POSX		DMM_COLUMN1_POSX + 25
#define DMM_ROW1_POSY			64 / 2 - 10
#define DMM_ROW2_POSY			64 / 2 + INDICATORS_FONT_HEIGHT + 7

class ScreenDMM : public ScreenBase
{
	private:
		UserIndicatorProgressBar<float> _indDMM1;
		UserIndicatorProgressBar<float> _indDMM2;
	
	public:
		ScreenDMM() : ScreenBase("DMM"),
			_indDMM1(DMM_COLUMN2_POSX, DMM_ROW1_POSY, &DevStatus.DMM1, "V", 0, 20, 100, 10),
			_indDMM2(DMM_COLUMN2_POSX, DMM_ROW2_POSY, &DevStatus.DMM2, "V", 0, 20, 100, 10)
		{
			_userIndicators[0] = &_indDMM1;
			_userIndicators[1] = &_indDMM2;
			_numUserIndicators = 2;
		}

		virtual void Draw(u8g_t* u8g, DevStatus_t devStatusDraw);
};

#endif /* SCREENDMM_H_ */