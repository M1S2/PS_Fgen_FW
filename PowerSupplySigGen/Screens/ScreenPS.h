/*
 * ScreenPS.h
 *
 * Created: 07.11.2020 13:09:43
 *  Author: V17
 */ 


#ifndef SCREENPS_H_
#define SCREENPS_H_

#include "ScreenBase.h"
#include "../UserControls/UserControlBool.h"
#include "../UserControls/UserControlNumeric.h"
#include "../Global/DevSettings.h"

#define VOLTAGE_CONTROL_POSX		SCREEN_TAB_WIDTH + 10
#define VOLTAGE_CONTROL_POSY		10			//(64 / 2) - 7
#define OUTPUT_STATE_CONTROL_POSX	VOLTAGE_CONTROL_POSX
#define OUTPUT_STATE_CONTROL_POSY	40		//(64 / 2) + CONTROLS_FONT_HEIGHT + 7
#define INFO_TEXTS_FONT_HEIGHT		10														// 10 pixel height font
#define INFO_TEXTS_POSX				VOLTAGE_CONTROL_POSX + 7 * 7 + 60						// voltage control uses 7 pixel width font
#define INFO_TEXT_VOLTAGE_POSY		(64 / 2) + INFO_TEXTS_FONT_HEIGHT / 2
#define INFO_TEXT_CURRENT_POSY		INFO_TEXT_VOLTAGE_POSY - INFO_TEXTS_FONT_HEIGHT - 5
#define INFO_TEXT_POWER_POSY		INFO_TEXT_VOLTAGE_POSY + INFO_TEXTS_FONT_HEIGHT + 5

class ScreenPS : public ScreenBase
{
	private:
		UserControlNumeric<uint16_t> _ctrlPSVoltage;
		UserControlBool _ctrlOutputEnable;
	
	public:
		ScreenPS() : ScreenBase("PS"), 
			_ctrlPSVoltage(VOLTAGE_CONTROL_POSX, VOLTAGE_CONTROL_POSY, &DevSettings.PS_Voltage_mV, "V", -3, 0, 10000),
			_ctrlOutputEnable(OUTPUT_STATE_CONTROL_POSX, OUTPUT_STATE_CONTROL_POSY, &DevSettings.PS_Output_Enabled)			
		{
			_ctrlPSVoltage.IsSelected = true;
			
			_userControls[0] = &_ctrlOutputEnable;
			_userControls[1] = &_ctrlPSVoltage;
			_numUserControls = 2;
		}

		virtual void Draw(u8g_t* u8g, DevStatus_t devStatusDraw);
};

#endif /* SCREENPS_H_ */