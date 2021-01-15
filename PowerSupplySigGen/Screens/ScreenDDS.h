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
#include "../Outputs/DDS_Channel.h"

#define SIGNALFORM_CONTROL_POSX			SCREEN_TAB_WIDTH + 10
#define SIGNALFORM_CONTROL_POSY			5
#define FREQUENCY_CONTROL_POSX			SCREEN_TAB_WIDTH + 10
#define FREQUENCY_CONTROL_POSY			25
#define ENABLED_CONTROL_POSX			SCREEN_TAB_WIDTH + 10
#define ENABLED_CONTROL_POSY			45
#define AMPLITUDE_CONTROL_POSX			SCREEN_TAB_WIDTH + 90
#define AMPLITUDE_CONTROL_POSY			5
#define OFFSET_CONTROL_POSX				SCREEN_TAB_WIDTH + 90
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
		ScreenDDS() : ScreenBase("DDS1"),
			_ctrlDDSSignalForm(SIGNALFORM_CONTROL_POSX, SIGNALFORM_CONTROL_POSY, &DDS_Channel1.SignalForm, SignalFormsNames, 4, &DDSUpdateSignalFormsCallbackFunction),
			_ctrlDDSFrequency(FREQUENCY_CONTROL_POSX, FREQUENCY_CONTROL_POSY, &DDS_Channel1.Frequency, "Hz", 0, 1, 100000.0f, &DDSUpdateIncrementsCallbackFunction),
			_ctrlDDSEnabled(ENABLED_CONTROL_POSX, ENABLED_CONTROL_POSY, &DDS_Channel1.Enabled, &DDSUpdateEnabledCallbackFunction),
			_ctrlDDSAmplitude(AMPLITUDE_CONTROL_POSX, AMPLITUDE_CONTROL_POSY, &DDS_Channel1.Amplitude, "Vpp", 0, 0, DDS_AMPLITUDE_MAX, &DDSUpdateWaveTableCallbackFunction),
			_ctrlDDSOffset(OFFSET_CONTROL_POSX, OFFSET_CONTROL_POSY, &DDS_Channel1.Offset, "V", 0, -DDS_AMPLITUDE_MAX, DDS_AMPLITUDE_MAX, &DDSUpdateWaveTableCallbackFunction)
		{
			_ctrlDDSFrequency.IsSelected = true;
		
			_userControls[0] = &_ctrlDDSSignalForm;
			_userControls[1] = &_ctrlDDSFrequency;
			_userControls[2] = &_ctrlDDSEnabled;
			_userControls[3] = &_ctrlDDSAmplitude;
			_userControls[4] = &_ctrlDDSOffset;
			_numUserControls = 5;
		}

		virtual void Draw(u8g_t* u8g, DevStatus_t devStatusDraw);
};

#endif /* SCREENDDS_H_ */