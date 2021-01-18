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
#include "../Global/DevSettings.h"
#include "../Global/DevStatus.h"
#include "../Outputs/PowerSupply.h"

#define VOLTAGE_CONTROL_POSX			SCREEN_TAB_WIDTH + 10
#define VOLTAGE_CONTROL_POSY			5			//(64 / 2) - 7
#define LOAD_IMPEDANCE_CONTROL_POSX		SCREEN_TAB_WIDTH + 10
#define LOAD_IMPEDANCE_CONTROL_POSY		25
#define OUTPUT_STATE_CONTROL_POSX		VOLTAGE_CONTROL_POSX
#define OUTPUT_STATE_CONTROL_POSY		45		//(64 / 2) + CONTROLS_FONT_HEIGHT + 7
#define INFO_TEXTS_POSX					VOLTAGE_CONTROL_POSX + 7 * 7 + 60						// voltage control uses 7 pixel width font
#define INFO_TEXT_CURRENT_POSY			(64 / 2) - INDICATORS_FONT_HEIGHT / 2
#define INFO_TEXT_VOLTAGE_POSY			INFO_TEXT_CURRENT_POSY - INDICATORS_FONT_HEIGHT - 5
#define INFO_TEXT_POWER_POSY			INFO_TEXT_CURRENT_POSY + INDICATORS_FONT_HEIGHT + 5

class ScreenPS : public ScreenBase
{
	private:
		UserControlNumeric<float> _ctrlPSVoltage;
		UserControlNumeric<float> _ctrlLoadImpedance;
		UserControlBool _ctrlOutputEnable;
		
		UserIndicatorNumeric<float> _indPSVoltage;
		UserIndicatorNumeric<float> _indPSCurrent;
		UserIndicatorNumeric<float> _indPSPower;
	
	public:
		ScreenPS() : ScreenBase("PS"), 
			_ctrlPSVoltage(VOLTAGE_CONTROL_POSX, VOLTAGE_CONTROL_POSY, &PowerSupply.Voltage, "V", 0, 0, 10, &PSUpdateOutputCallbackFunction),
			_ctrlLoadImpedance(LOAD_IMPEDANCE_CONTROL_POSX, LOAD_IMPEDANCE_CONTROL_POSY, &PowerSupply.LoadImpedance, "Ohm", 0, PS_MIN_LOAD_IMPEDANCE, 1000000, &PSUpdateOutputCallbackFunction),
			_ctrlOutputEnable(OUTPUT_STATE_CONTROL_POSX, OUTPUT_STATE_CONTROL_POSY, &PowerSupply.OutputEnabled, &PSUpdateOutputCallbackFunction),
			_indPSVoltage(INFO_TEXTS_POSX, INFO_TEXT_VOLTAGE_POSY, &DevStatusDraw.PS_VOLT, "V"),
			_indPSCurrent(INFO_TEXTS_POSX, INFO_TEXT_CURRENT_POSY, &DevStatusDraw.PS_CURR, "A"),
			_indPSPower(INFO_TEXTS_POSX, INFO_TEXT_POWER_POSY, &DevStatusDraw.PS_POWER, "W")
		{
			_ctrlPSVoltage.IsSelected = true;
			
			_userControls[0] = &_ctrlPSVoltage;
			_userControls[1] = &_ctrlLoadImpedance;
			_userControls[2] = &_ctrlOutputEnable;
			_numUserControls = 3;
			
			_userIndicators[0] = &_indPSVoltage;
			_userIndicators[1] = &_indPSCurrent;
			_userIndicators[2] = &_indPSPower;
			_numUserIndicators = 3;
		}

		virtual void Draw(u8g_t* u8g);
};

#endif /* SCREENPS_H_ */