/*
 * ScreenDDS.h
 *
 * Created: 07.01.2021 19:35:05
 *  Author: V17
 */ 


#ifndef SCREENDDS_H_
#define SCREENDDS_H_

#include "ScreenBase.h"
#include "../UserControls/UserControlBool.h"
#include "../UserControls/UserControlNumeric.h"
#include "../UserControls/UserControlEnum.h"
#include "../Outputs/DDS_Channel.h"

#define SIGNALFORM_CONTROL_POSX			SCREEN_TAB_WIDTH + 10
#define SIGNALFORM_CONTROL_POSY			5
#define FREQUENCY_CONTROL_POSX			SCREEN_TAB_WIDTH + 10
#define FREQUENCY_CONTROL_POSY			25

class ScreenDDS : public ScreenBase
{
	private:
		UserControlEnum<SignalForms_t> _ctrlDDSSignalForm;
		UserControlNumeric<float> _ctrlDDSFrequency;
	
	public:
		ScreenDDS() : ScreenBase("DDS1"),
			_ctrlDDSSignalForm(SIGNALFORM_CONTROL_POSX, SIGNALFORM_CONTROL_POSY, &DDS_Channel1.SignalForm, SignalFormsNames, 4, &DDSUpdateSignalFormsCallbackFunction),
			_ctrlDDSFrequency(FREQUENCY_CONTROL_POSX, FREQUENCY_CONTROL_POSY, &DDS_Channel1.Frequency, "Hz", 0, 1, 100000.0f, &DDSUpdateIncrementsCallbackFunction)
		{
			_ctrlDDSFrequency.IsSelected = true;
		
			_userControls[0] = &_ctrlDDSSignalForm;
			_userControls[1] = &_ctrlDDSFrequency;
			_numUserControls = 2;
		}

		virtual void Draw(u8g_t* u8g, DevStatus_t devStatusDraw);
};

#endif /* SCREENDDS_H_ */