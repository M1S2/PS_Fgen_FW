/*
 * ScreenCalibration.cpp
 *
 * Created: 13.05.2021 20:59:49
 *  Author: V17
 */ 

#include "../Device.h"

typedef enum CalibrationStates
{
	CAL_REF_VOLTAGE,
	CAL_ATX_3V3,
	//...
	NUM_CAL_STEPS		// The last element is used to determine the number of elements in the enumeration
}CalibrationStates_t;
CalibrationStates_t CalState;
uint8_t CalStateNumber;			// 1 .. Number calibration steps

float CalTmpVoltage;				// Variable holding the entered voltage value for each calibration step. This is converted to the corresponding factor in the OnButtonCalNext() function


#define CAL_CAPTION_POSX	25
#define CAL_BUTTONS_POSY	50

void OnButtonCalExit(void* context);
void OnButtonCalNext(void* context);
void OnCalFinishedOK(void* context);

ContainerPage page_Cal;
Icon ico_Cal(5, 3, icon_calibration_width, icon_calibration_height, icon_calibration_bits);
Label<40> lbl_Cal_caption(CAL_CAPTION_POSX, 5, "Calibration (measure with a multimeter)");
Label<30> lbl_Cal_instruction(CAL_CAPTION_POSX, 20, "...");
NumericControl<float> numCtrl_Cal_tmpVoltage(CAL_CAPTION_POSX, 35, &CalTmpVoltage, "V", 0, 15, 3);

ButtonControl<5> button_Cal_Exit(160, CAL_BUTTONS_POSY, 30, DEFAULT_UI_ELEMENT_HEIGHT, "Exit", NULL, &OnButtonCalExit);
ButtonControl<5> button_Cal_Next(200, CAL_BUTTONS_POSY, 30, DEFAULT_UI_ELEMENT_HEIGHT, "Next", NULL, &OnButtonCalNext);
ProgressBar<uint8_t> progress_Cal(5, CAL_BUTTONS_POSY + 5, 130, 5, &CalStateNumber, 0, NUM_CAL_STEPS, PROGRESSBAR_ORIGIN_ZERO, 0);
MessageDialog msg_Cal_Finished(0, 0, 240, 64, "Calibration finished.", MSG_INFO, MSG_BTN_OK, NULL, &OnCalFinishedOK);


UIElement* StartCalibration()
{
	CalState = CAL_REF_VOLTAGE;
	CalStateNumber = (uint8_t)CalState;
	CalTmpVoltage = Device.CalibrationFactors.Cal_RefVoltage;
	lbl_Cal_instruction.SetText("Voltage at the +5V output:");
	return &page_Cal;
}

void OnButtonCalNext(void* context)
{
	switch(CalState)
	{
		case CAL_REF_VOLTAGE:
		{
			// Do reference voltage calibration
			Device.CalibrationFactors.Cal_RefVoltage = CalTmpVoltage;
			
			CalState = CAL_ATX_3V3;
			CalTmpVoltage = Device.DeviceVoltages.ATX_3V3;
			lbl_Cal_instruction.SetText("Voltage at the +3V3 output:");
			break;
		}
		case CAL_ATX_3V3: 
		{
			// Do 3V3 calibration
			//Device.CalibrationFactors.Cal_ATX_3V3 = 
			
			Device.SaveSettings();
			Device.ScreenManager.UiManager.ChangeVisualTreeRoot(&msg_Cal_Finished);
			break;
		}
	}
	CalStateNumber = (uint8_t)CalState;
}

void OnButtonCalExit(void* context)
{
	Device.ScreenManager.ShowUiMainPage();
}

void OnCalFinishedOK(void* context)
{
	Device.ScreenManager.ShowUiMainPage();
}

UIElement* uiBuildScreenCalibration()
{
	page_Cal.AddItem(&lbl_Cal_caption);
	page_Cal.AddItem(&ico_Cal);
	page_Cal.AddItem(&lbl_Cal_instruction);
	page_Cal.AddItem(&numCtrl_Cal_tmpVoltage);
	page_Cal.AddItem(&progress_Cal);
	page_Cal.AddItem(&button_Cal_Exit);
	page_Cal.AddItem(&button_Cal_Next);
	page_Cal.InitItems();
	
	return &page_Cal;
}