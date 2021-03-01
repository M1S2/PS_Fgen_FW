/*
 * ScreenPS.h
 *
 * Created: 07.11.2020 13:09:43
 *  Author: V17
 */ 


#ifndef SCREENPS_H_
#define SCREENPS_H_

#include "ScreenBase.h"
#include "../UserControlsIndicators/UserControlBool.h"
#include "../UserControlsIndicators/UserControlNumeric.h"
#include "../UserControlsIndicators/UserIndicatorNumeric.h"
#include "../UserControlsIndicators/UserIndicatorEnum.h"


#define VOLTAGE_CONTROL_POSX			SCREEN_TAB_WIDTH + 10
#define VOLTAGE_CONTROL_POSY			5			//(64 / 2) - 7
#define LOAD_IMPEDANCE_CONTROL_POSX		SCREEN_TAB_WIDTH + 10
#define LOAD_IMPEDANCE_CONTROL_POSY		25
#define OUTPUT_STATE_CONTROL_POSX		VOLTAGE_CONTROL_POSX
#define OUTPUT_STATE_CONTROL_POSY		45		//(64 / 2) + CONTROLS_FONT_HEIGHT + 7
#define INFO_TEXTS_POSX					VOLTAGE_CONTROL_POSX + 7 * 7 + 60						// voltage control uses 7 pixel width font
#define INFO_TEXT_VOLTAGE_POSY			10
#define INFO_TEXT_CURRENT_POSY			INFO_TEXT_VOLTAGE_POSY + INDICATORS_FONT_HEIGHT + 5
#define INFO_TEXT_POWER_POSY			INFO_TEXT_CURRENT_POSY + INDICATORS_FONT_HEIGHT + 5
#define INFO_TEXT_STATE_POSY			INFO_TEXT_POWER_POSY + INDICATORS_FONT_HEIGHT + 5

class ScreenPS : public ScreenBase
{
	private:
		UserControlNumeric<float> _ctrlPSVoltage;
		UserControlNumeric<float> _ctrlLoadImpedance;
		UserControlBool _ctrlOutputEnable;
		
		UserIndicatorNumeric<float> _indPSVoltage;
		UserIndicatorNumeric<float> _indPSCurrent;
		UserIndicatorNumeric<float> _indPSPower;
		UserIndicatorEnum<PsStates_t> _indPSState;
	
	public:
		ScreenPS();
		virtual void Draw(u8g_t* u8g, bool isFirstPage);
};

#endif /* SCREENPS_H_ */