/*
 * ScreenDDS.h
 *
 * Created: 07.01.2021 19:35:05
 *  Author: V17
 */ 


#ifndef SCREENDDS_H_
#define SCREENDDS_H_

#include "ScreenBase.h"
#include "../UserControlsIndicators/UserControlBool.h"
#include "../UserControlsIndicators/UserControlNumeric.h"
#include "../UserControlsIndicators/UserControlEnum.h"

#define SIGNALFORM_CONTROL_POSX			SCREEN_TAB_WIDTH + 5
#define SIGNALFORM_CONTROL_POSY			5
#define FREQUENCY_CONTROL_POSX			SIGNALFORM_CONTROL_POSX
#define FREQUENCY_CONTROL_POSY			25
#define ENABLED_CONTROL_POSX			SIGNALFORM_CONTROL_POSX
#define ENABLED_CONTROL_POSY			45
#define AMPLITUDE_CONTROL_POSX			SCREEN_TAB_WIDTH + 90
#define AMPLITUDE_CONTROL_POSY			5
#define OFFSET_CONTROL_POSX				AMPLITUDE_CONTROL_POSX
#define OFFSET_CONTROL_POSY				25

class ScreenDDS : public ScreenBase
{
	private:
		UserControlEnum<SignalForms_t> _ctrlDDSSignalForm;
		UserControlNumeric<float> _ctrlDDSFrequency;
		UserControlBool _ctrlDDSEnabled;
		UserControlNumeric<float> _ctrlDDSAmplitude;
		UserControlNumeric<float> _ctrlDDSOffset;
			
	public:
		ScreenDDS();
		virtual void Draw(u8g_t* u8g, bool isFirstPage);
};

#endif /* SCREENDDS_H_ */