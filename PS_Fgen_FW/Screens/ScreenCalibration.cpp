/*
 * ScreenCalibration.cpp
 * Created: 13.05.2021 20:59:49
 * Author: Markus Scheich
 */ 

#include "../Device.h"

typedef enum CalibrationStates
{
	CAL_REF_VOLTAGE_ATX_5V,			/**< Calibrate the reference voltage and the ATX 5V voltage */
	CAL_ATX_3V3,					/**< Calibrate the ATX 3V3 voltage */
	CAL_ATX_12V,					/**< Calibrate the ATX 12V voltage */
	CAL_ATX_12V_NEG,				/**< Calibrate the ATX 12V NEG voltage */
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		CAL_DMM1,					/**< Calibrate the DMM1 voltage */
		CAL_DMM2,					/**< Calibrate the DMM2 voltage */
	#endif
	#if defined MEASURE_SUBSYSTEM_ENABLED && defined PS_SUBSYSTEM_ENABLED 
		CAL_PS_VOLT,				/**< Calibrate the PS_VOLT voltage */
	#endif
	#if defined MEASURE_SUBSYSTEM_ENABLED && defined DDS_SUBSYSTEM_ENABLED
		CAL_DDS_FREQ,				/**< Calibrate the DDS frequency */
	#endif
	//...
	NUM_CAL_STEPS					/**< The last element is used to determine the number of elements in the enumeration */
}CalibrationStates_t;
CalibrationStates_t CalState;
uint8_t CalStateNumber;			// 1 .. Number calibration steps

float CalTmpVoltage;				// Variable holding the entered voltage value for each calibration step. This is converted to the corresponding factor in the OnButtonCalNext() function
float CalTmpFrequency;				// Variable holding the entered frequency value for the DDS frequency calibration step. This is converted to the corresponding factor in the OnButtonCalNext() function


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
NumericControl<float> numCtrl_Cal_tmpFrequency(CAL_CAPTION_POSX, 35, &CalTmpFrequency, "Hz", 1, 20000, 1);

ButtonControl<5> button_Cal_Exit(160, CAL_BUTTONS_POSY, 30, DEFAULT_UI_ELEMENT_HEIGHT, "Exit", NULL, &OnButtonCalExit);
ButtonControl<5> button_Cal_Next(200, CAL_BUTTONS_POSY, 30, DEFAULT_UI_ELEMENT_HEIGHT, "Next", NULL, &OnButtonCalNext);
ProgressBar<uint8_t> progress_Cal(5, CAL_BUTTONS_POSY + 5, 130, 5, &CalStateNumber, 0, NUM_CAL_STEPS, PROGRESSBAR_ORIGIN_ZERO, 0);
MessageDialog<25> msg_Cal_Finished(0, 0, 240, 64, "Calibration finished.", MSG_INFO, MSG_BTN_OK, NULL, &OnCalFinishedOK);

/**
 * Prepare everything for the given calibration state.
 * This includes setting the correct message and configuring all needed channels.
 * @param calState Calibration state for which to prepare everything.
 */
void PrepareCalStep(CalibrationStates_t calState)
{
	switch(calState)
	{
		case CAL_REF_VOLTAGE_ATX_5V:
		{
			CalTmpVoltage = Device.CalibrationFactors.Cal_RefVoltage;
			numCtrl_Cal_tmpVoltage.Visible = true;
			numCtrl_Cal_tmpFrequency.Visible = false;
			lbl_Cal_instruction.SetText("Measure voltage at the +5V output:");
			break;
		}
		case CAL_ATX_3V3:
		{
			CalTmpVoltage = Device.DeviceVoltages.ATX_3V3;
			lbl_Cal_instruction.SetText("Measure voltage at the +3V3 output:");
			break;
		}
		case CAL_ATX_12V:
		{
			CalTmpVoltage = Device.DeviceVoltages.ATX_12V;
			lbl_Cal_instruction.SetText("Measure voltage at the +12V output:");
			break;
		}
		case CAL_ATX_12V_NEG:
		{
			CalTmpVoltage = Device.DeviceVoltages.ATX_12V_NEG;
			lbl_Cal_instruction.SetText("Measure voltage at the -12V output:");
			break;
		}
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		case CAL_DMM1:
		{
			numCtrl_Cal_tmpVoltage.Visible = false;
			lbl_Cal_instruction.SetText("Connect +12V output to DMM1 input.");
			break;
		}
		case CAL_DMM2:
		{
			lbl_Cal_instruction.SetText("Connect +12V output to DMM2 input.");
			break;
		}
	#endif
	#if defined MEASURE_SUBSYSTEM_ENABLED && defined PS_SUBSYSTEM_ENABLED
		case CAL_PS_VOLT:
		{
			Device.PsChannel.SetVoltage(10);		// Set output to 10V and enable it
			Device.PsChannel.SetEnabled(true);
			lbl_Cal_instruction.SetText("Connect PS+ output to DMM1 input.");
			break;
		}
	#endif
	#if defined MEASURE_SUBSYSTEM_ENABLED && defined DDS_SUBSYSTEM_ENABLED
		case CAL_DDS_FREQ:
		{
			CalTmpFrequency = 1000;
			Device.DdsChannel1.SetAmplitude(20);		// Set output to 10 V, 1 kHz and enable it
			Device.DdsChannel1.SetFrequency(CalTmpFrequency);
			Device.DdsChannel1.SetEnabled(true);
			numCtrl_Cal_tmpFrequency.Visible = true;
			lbl_Cal_instruction.SetText("Measure frequency at the DDS1 output:");
			break;
		}
	#endif
		default: break;
	}
	CalState = calState;
}

/** 
 * Save all calibration factors and show a message that the calibration is finished. 
 */
void CalibrationFinished()
{
	Device.SaveSettingsCalibrationFactors();
	Device.ReportCalibrationFactors();
	Device.ScreenManager.UiManager.ChangeVisualTreeRoot(&msg_Cal_Finished);
}

/** 
 * Start a new calibration.
 * This initializes all UI elements to the neccessary states and returns a pointer to the calibration screen.
 * @return Pointer to the calibration screen. Use this to display the calibration screen. 
 */
UIElement* StartCalibration()
{
	PrepareCalStep(CAL_REF_VOLTAGE_ATX_5V);
	CalStateNumber = (uint8_t)CalState;
	return &page_Cal;
}

void OnButtonCalNext(void* context)
{	
	switch(CalState)
	{
		case CAL_REF_VOLTAGE_ATX_5V:
		{
			// Do reference voltage calibration
			Device.CalibrationFactors.Cal_RefVoltage = CalTmpVoltage;
			// Do 5V calibration
			Device.CalibrationFactors.Cal_ATX_5V *= (CalTmpVoltage / Device.DeviceVoltages.ATX_5V); 
			
			PrepareCalStep(CAL_ATX_3V3);
			break;
		}
		case CAL_ATX_3V3: 
		{
			// Do 3V3 calibration
			Device.CalibrationFactors.Cal_ATX_3V3 *= (CalTmpVoltage / Device.DeviceVoltages.ATX_3V3);
						
			PrepareCalStep(CAL_ATX_12V);
			break;
		}
		case CAL_ATX_12V:
		{
			// Do 12V calibration
			Device.CalibrationFactors.Cal_ATX_12V *= (CalTmpVoltage / Device.DeviceVoltages.ATX_12V);
			
			PrepareCalStep(CAL_ATX_12V_NEG);
			break;
		}
		case CAL_ATX_12V_NEG:
		{
			// Do 12V NEG calibration
			Device.CalibrationFactors.Cal_ATX_12V_NEG *= (CalTmpVoltage / Device.DeviceVoltages.ATX_12V_NEG);
			
			#ifdef MEASURE_SUBSYSTEM_ENABLED
				PrepareCalStep(CAL_DMM1);
			#else
				CalibrationFinished();
			#endif
			break;
		}
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		case CAL_DMM1:
		{
			// Do DMM1 calibration
			Device.CalibrationFactors.Cal_DMM1 *= (Device.DeviceVoltages.ATX_12V / Device.DmmChannel1.MeasuredVoltage);
			
			PrepareCalStep(CAL_DMM2);
			break;			
		}
		case CAL_DMM2:
		{
			// Do DMM2 calibration
			Device.CalibrationFactors.Cal_DMM2 *= (Device.DeviceVoltages.ATX_12V / Device.DmmChannel2.MeasuredVoltage);			
			
			#ifdef PS_SUBSYSTEM_ENABLED 
				PrepareCalStep(CAL_PS_VOLT);
			#elif defined DDS_SUBSYSTEM_ENABLED
				PrepareCalStep(CAL_DDS_FREQ);
			#else
				CalibrationFinished();
			#endif
			break;
		}
	#endif
	#if defined MEASURE_SUBSYSTEM_ENABLED && defined PS_SUBSYSTEM_ENABLED 
		case CAL_PS_VOLT:
		{
			// Do PS_VOLT calibration
			Device.CalibrationFactors.Cal_PS_VOLT *= (Device.DmmChannel1.MeasuredVoltage / Device.PsChannel.MeasuredVoltage);
			
			Device.PsChannel.SetEnabled(false);
			
			#ifdef DDS_SUBSYSTEM_ENABLED 
				PrepareCalStep(CAL_DDS_FREQ);
			#else
				CalibrationFinished();
			#endif
			break;
		}
	#endif
	#if defined MEASURE_SUBSYSTEM_ENABLED && defined DDS_SUBSYSTEM_ENABLED 
		case CAL_DDS_FREQ:
		{
			// Do DDS frequency calibration
			Device.CalibrationFactors.Cal_DDS_FREQ = (1000.0f / CalTmpFrequency);		// 1000 = 1000 kHz desired frequency
			
			Device.DdsChannel1.UpdateIncrement();
			Device.DdsChannel2.UpdateIncrement();
			Device.DdsChannel1.SetEnabled(false);
			
			CalibrationFinished();
			break;
		}
	#endif
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
	numCtrl_Cal_tmpFrequency.Visible = false;
	
	page_Cal.AddItem(&lbl_Cal_caption);
	page_Cal.AddItem(&ico_Cal);
	page_Cal.AddItem(&lbl_Cal_instruction);
	page_Cal.AddItem(&numCtrl_Cal_tmpVoltage);
	page_Cal.AddItem(&numCtrl_Cal_tmpFrequency);
	page_Cal.AddItem(&progress_Cal);
	page_Cal.AddItem(&button_Cal_Exit);
	page_Cal.AddItem(&button_Cal_Next);
	page_Cal.InitItems();
	
	return &page_Cal;
}