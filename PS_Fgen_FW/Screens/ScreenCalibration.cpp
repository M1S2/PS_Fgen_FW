/*
 * ScreenCalibration.cpp
 *
 * Created: 13.05.2021 20:59:49
 *  Author: V17
 */ 

#include "../Device.h"

#include "../USART/USART.h"

typedef enum CalibrationStates
{
	CAL_REF_VOLTAGE_ATX_5V,		// Calibrate the reference voltage and the ATX 5V voltage
	CAL_ATX_3V3,				// Calibrate the ATX 3V3 voltage
	CAL_ATX_12V,				// Calibrate the ATX 12V voltage
	CAL_ATX_12V_NEG,			// Calibrate the ATX 12V NEG voltage
	CAL_DMM1,					// Calibrate the DMM1 voltage
	CAL_DMM2,					// Calibrate the DMM2 voltage
	CAL_PS_VOLT,				// Calibrate the PS_VOLT voltage
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
Label<15> lbl_Cal_caption(CAL_CAPTION_POSX, 5, "Calibration");
Label<40> lbl_Cal_instruction(CAL_CAPTION_POSX, 20, "...");
NumericControl<float> numCtrl_Cal_tmpVoltage(CAL_CAPTION_POSX, 35, &CalTmpVoltage, "V", -15, 15, 3);

ButtonControl<5> button_Cal_Exit(160, CAL_BUTTONS_POSY, 30, DEFAULT_UI_ELEMENT_HEIGHT, "Exit", NULL, &OnButtonCalExit);
ButtonControl<5> button_Cal_Next(200, CAL_BUTTONS_POSY, 30, DEFAULT_UI_ELEMENT_HEIGHT, "Next", NULL, &OnButtonCalNext);
ProgressBar<uint8_t> progress_Cal(5, CAL_BUTTONS_POSY + 5, 130, 5, &CalStateNumber, 0, NUM_CAL_STEPS, PROGRESSBAR_ORIGIN_ZERO, 0);
MessageDialog msg_Cal_Finished(0, 0, 240, 64, "Calibration finished.", MSG_INFO, MSG_BTN_OK, NULL, &OnCalFinishedOK);


UIElement* StartCalibration()
{
	CalState = CAL_REF_VOLTAGE_ATX_5V;
	CalStateNumber = (uint8_t)CalState;
	CalTmpVoltage = Device.CalibrationFactors.Cal_RefVoltage;
	numCtrl_Cal_tmpVoltage.Visible = true;
	lbl_Cal_instruction.SetText("Measure voltage at the +5V output:");
	return &page_Cal;
}

void OnButtonCalNext(void* context)
{
	char buffer[50];
	
	switch(CalState)
	{
		case CAL_REF_VOLTAGE_ATX_5V:
		{
			// Do reference voltage calibration
			Device.CalibrationFactors.Cal_RefVoltage = CalTmpVoltage;
			// Do 5V calibration
			Device.CalibrationFactors.Cal_ATX_5V *= (CalTmpVoltage / Device.DeviceVoltages.ATX_5V); 
			
			/*dtostrf(Device.CalibrationFactors.Cal_RefVoltage, 10, 3, buffer);
			Usart0TransmitStr("Ref=");
			Usart0TransmitStr(buffer);
			Usart0TransmitStr("\r\n5V=");
			dtostrf(Device.CalibrationFactors.Cal_ATX_5V, 10, 3, buffer);
			Usart0TransmitStr(buffer);*/
			
			CalState = CAL_ATX_3V3;
			CalTmpVoltage = Device.DeviceVoltages.ATX_3V3;
			lbl_Cal_instruction.SetText("Measure voltage at the +3V3 output:");
			break;
		}
		case CAL_ATX_3V3: 
		{
			// Do 3V3 calibration
			Device.CalibrationFactors.Cal_ATX_3V3 *= (CalTmpVoltage / Device.DeviceVoltages.ATX_3V3);
			
			/*Usart0TransmitStr("\r\n3V3=");
			dtostrf(Device.CalibrationFactors.Cal_ATX_3V3, 10, 3, buffer);
			Usart0TransmitStr(buffer);*/
						
			CalState = CAL_ATX_12V;
			CalTmpVoltage = Device.DeviceVoltages.ATX_12V;
			lbl_Cal_instruction.SetText("Measure voltage at the +12V output:");
			break;
		}
		case CAL_ATX_12V:
		{
			// Do 12V calibration
			Device.CalibrationFactors.Cal_ATX_12V *= (CalTmpVoltage / Device.DeviceVoltages.ATX_12V);
			
			/*Usart0TransmitStr("\r\n12V=");
			dtostrf(Device.CalibrationFactors.Cal_ATX_12V, 10, 3, buffer);
			Usart0TransmitStr(buffer);*/
			
			CalState = CAL_ATX_12V_NEG;
			CalTmpVoltage = Device.DeviceVoltages.ATX_12V_NEG;
			lbl_Cal_instruction.SetText("Measure voltage at the -12V output:");
			break;
		}
		case CAL_ATX_12V_NEG:
		{
			// Do 12V NEG calibration
			Device.CalibrationFactors.Cal_ATX_12V_NEG *= (CalTmpVoltage / Device.DeviceVoltages.ATX_12V_NEG);
			
			/*Usart0TransmitStr("\r\n-12V=");
			dtostrf(Device.CalibrationFactors.Cal_ATX_12V_NEG, 10, 3, buffer);
			Usart0TransmitStr(buffer);*/
			
			CalState = CAL_DMM1;
			numCtrl_Cal_tmpVoltage.Visible = false;
			lbl_Cal_instruction.SetText("Connect +12V output to DMM1 input.");
			break;
		}
		case CAL_DMM1:
		{
			// Do DMM1 calibration
			Device.CalibrationFactors.Cal_DMM1 *= (Device.DeviceVoltages.ATX_12V / Device.DmmChannel1.MeasuredVoltage);
				
			/*Usart0TransmitStr("\r\nDMM1=");
			dtostrf(Device.CalibrationFactors.Cal_DMM1, 10, 3, buffer);
			Usart0TransmitStr(buffer);*/
			
			CalState = CAL_DMM2;
			lbl_Cal_instruction.SetText("Connect +12V output to DMM2 input.");
			break;			
		}
		case CAL_DMM2:
		{
			// Do DMM2 calibration
			Device.CalibrationFactors.Cal_DMM2 *= (Device.DeviceVoltages.ATX_12V / Device.DmmChannel2.MeasuredVoltage);
			
			/*Usart0TransmitStr("\r\nDMM2=");
			dtostrf(Device.CalibrationFactors.Cal_DMM2, 10, 3, buffer);
			Usart0TransmitStr(buffer);*/			
			
			CalState = CAL_PS_VOLT;
			Device.PsChannel.SetAmplitude(10);		// Set output to 10V and enable it
			Device.PsChannel.SetEnabled(true);
			lbl_Cal_instruction.SetText("Connect PS+ output to DMM1 input.");
			break;
		}
		case CAL_PS_VOLT:
		{
			// Do PS_VOLT calibration
			Device.CalibrationFactors.Cal_PS_VOLT *= (Device.DmmChannel1.MeasuredVoltage / Device.PsChannel.MeasuredAmplitude);
			
			/*Usart0TransmitStr("\r\nPS_VOLT=");
			dtostrf(Device.CalibrationFactors.Cal_PS_VOLT, 10, 3, buffer);
			Usart0TransmitStr(buffer);*/
			
			Device.PsChannel.SetEnabled(false);
			numCtrl_Cal_tmpVoltage.Visible = true;
			
			Device.SaveSettings();
			Device.ScreenManager.UiManager.ChangeVisualTreeRoot(&msg_Cal_Finished);
			break;
		}
		default: break;
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