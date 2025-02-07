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
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		CAL_DMM1,					/**< Calibrate the DMM1 voltage */
		CAL_DMM2,					/**< Calibrate the DMM2 voltage */
	#endif
	#if defined MEASURE_SUBSYSTEM_ENABLED && defined PS_SUBSYSTEM_ENABLED 
		CAL_PS_VOLT,				/**< Calibrate the PS_VOLT voltage */
		CAL_PS_CURR,				/**< Calibrate the PS_CURR current */
		CAL_PS_CURR_OFFSET,			/**< Calibrate the PS_CURR offset current */
	#endif
	#if defined MEASURE_SUBSYSTEM_ENABLED && defined DDS_SUBSYSTEM_ENABLED
		CAL_DDS_FREQ,				/**< Calibrate the DDS frequency */
	#endif
	//...
	NUM_CAL_STEPS					/**< The last element is used to determine the number of elements in the enumeration */
}CalibrationStates_t;
CalibrationStates_t CalState;
uint8_t CalStateNumber;			// 1 .. Number calibration steps

float CalTmpVoltage;				/**< Variable holding the entered voltage value for each calibration step. This is converted to the corresponding factor in the OnButtonCalNext() function */
float CalTmpCurrent;				/**< Variable holding the entered current value for the PS_CURR calibration step. This is converted to the corresponding factor in the OnButtonCalNext() function */
float CalTmpFrequency;				/**< Variable holding the entered frequency value for the DDS frequency calibration step. This is converted to the corresponding factor in the OnButtonCalNext() function */


#define CAL_PS_CURR_OFFSET_CYCLES	1000	/**< Number of cycles that are used during the PS_CURR_OFFSET calibration step. The measured open circuit current is averaged over this number of cycles. */

void OnButtonCalExit(void* context);
void OnButtonCalNext(void* context);
void OnCalFinishedOK(void* context);

ContainerGrid grid_Cal(10, 5, 4, false, true);
Icon ico_Cal(icon_calibration_width, icon_calibration_height, icon_calibration_bits, COLOR_FOREGROUND_HEADERS);
Label lbl_Cal_caption("Calibration", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 15);
Label lbl_Cal_instruction("...", LABEL_COLOR_NOTSET, NULL, 0, 0, 45);
NumericControl<float> numCtrl_Cal_tmpVoltage(&CalTmpVoltage, "V", -15, 15, 3);
NumericControl<float> numCtrl_Cal_tmpCurrent(&CalTmpCurrent, "A", 0, 5, 3);
NumericControl<float> numCtrl_Cal_tmpFrequency(&CalTmpFrequency, "Hz", 1, 20000, 1);
ButtonControl button_Cal_Exit("Exit", NULL, &OnButtonCalExit, 5);
ButtonControl button_Cal_Next("Next", NULL, &OnButtonCalNext, 5);
Label lbl_NumCalSteps("# Steps:", LABEL_COLOR_NOTSET, NULL, 0, 0, 10);
ProgressBar<uint8_t> progress_Cal(&CalStateNumber, 0, NUM_CAL_STEPS, PROGRESSBAR_ORIGIN_ZERO, 0, 130, 20);
MessageDialog msg_Cal_Finished(MSG_DIALOG_MARGIN, MSG_DIALOG_MARGIN, DISPLAY_WIDTH - 2 * MSG_DIALOG_MARGIN, DISPLAY_HEIGHT - 2 * MSG_DIALOG_MARGIN, "Calibration finished.", MSG_INFO, MSG_BTN_OK, NULL, &OnCalFinishedOK, NULL, 25);

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
			numCtrl_Cal_tmpCurrent.Visible = false;
			numCtrl_Cal_tmpFrequency.Visible = false;
			lbl_Cal_instruction.SetText("Measure voltage at the\n+5V output:");
			break;
		}
		case CAL_ATX_3V3:
		{
			CalTmpVoltage = Device.DeviceVoltages.ATX_3V3;
			lbl_Cal_instruction.SetText("Measure voltage at the\n+3V3 output:");
			break;
		}
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		case CAL_DMM1:
		{
			numCtrl_Cal_tmpVoltage.Visible = false;
			lbl_Cal_instruction.SetText("Connect +5V output to\nDMM1 input.");
			break;
		}
		case CAL_DMM2:
		{
			lbl_Cal_instruction.SetText("Connect +5V output to\nDMM2 input.");
			break;
		}
	#endif
	#if defined MEASURE_SUBSYSTEM_ENABLED && defined PS_SUBSYSTEM_ENABLED
		case CAL_PS_VOLT:
		{
			Device.PsChannel.SetVoltage(5);		// Set output to 5V and enable it
			Device.PsChannel.SetEnabled(true);
			lbl_Cal_instruction.SetText("Connect PS+ output to\nDMM1 input.");
			break;
		}
		case CAL_PS_CURR:
		{
			// Power supply channel is already enabled in CAL_PS_VOLT step
			numCtrl_Cal_tmpCurrent.Visible = true;
			CalTmpCurrent = 2;
			Device.CalibrationFactors.Cal_PS_CURR_OFFSET = 0;
			lbl_Cal_instruction.SetText("Measure shortcut current\nof PS output:");
			break;
		}
		case CAL_PS_CURR_OFFSET:
		{
			numCtrl_Cal_tmpCurrent.Visible = false;
			// Power supply channel is already enabled in CAL_PS_VOLT step
			lbl_Cal_instruction.SetText("Disconnect everything\n(for current offset).");
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
			lbl_Cal_instruction.SetText("Measure frequency at\nthe DDS1 output:");
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
	Device.CalibrationFactors.Cal_Valid = true;
	Device.SaveSettingsCalibrationFactors();
	Device.ReportCalibrationFactors();
	UiManager.ChangeVisualTreeRoot(&msg_Cal_Finished);
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
	return &grid_Cal;
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
			Device.CalibrationFactors.Cal_ATX_5V *= (Device.DeviceVoltages.ATX_5V == 0 ? 1 : (CalTmpVoltage / Device.DeviceVoltages.ATX_5V)); 
			
			PrepareCalStep(CAL_ATX_3V3);
			break;
		}
		case CAL_ATX_3V3: 
		{
			// Do 3V3 calibration
			Device.CalibrationFactors.Cal_ATX_3V3 *= (Device.DeviceVoltages.ATX_3V3 == 0 ? 1 : (CalTmpVoltage / Device.DeviceVoltages.ATX_3V3));
						
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
			Device.CalibrationFactors.Cal_DMM1 *= (Device.DmmChannel1.MeasuredVoltage == 0 ? 1 : (Device.DeviceVoltages.ATX_5V / Device.DmmChannel1.MeasuredVoltage));
			
			PrepareCalStep(CAL_DMM2);
			break;			
		}
		case CAL_DMM2:
		{
			// Do DMM2 calibration
			Device.CalibrationFactors.Cal_DMM2 *= (Device.DmmChannel2.MeasuredVoltage == 0 ? 1 : (Device.DeviceVoltages.ATX_5V / Device.DmmChannel2.MeasuredVoltage));			
			
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
			Device.CalibrationFactors.Cal_PS_VOLT *= (Device.PsChannel.MeasuredVoltage == 0 ? 1 : (Device.DmmChannel1.MeasuredVoltage / Device.PsChannel.MeasuredVoltage));
			
			PrepareCalStep(CAL_PS_CURR);
			break;
		}
		case CAL_PS_CURR:
		{
			// Do PS_CURR calibration			
			Device.CalibrationFactors.Cal_PS_CURR *= (Device.PsChannel.MeasuredCurrent == 0 ? 1 : (CalTmpCurrent / Device.PsChannel.MeasuredCurrent));
			
			PrepareCalStep(CAL_PS_CURR_OFFSET);
			break;
		}
		case CAL_PS_CURR_OFFSET:
		{				
			// Do PS_CURR_OFFSET calibration
			float maxCurrent = 0;
			Device.CalibrationFactors.Cal_PS_CURR_OFFSET = 0;
			for(int i = 0; i < CAL_PS_CURR_OFFSET_CYCLES; i++)
			{				
				float measCurrent = Device.PsChannel.MeasuredCurrent;
				if(measCurrent > maxCurrent) { maxCurrent = measCurrent; }
			}
			Device.CalibrationFactors.Cal_PS_CURR_OFFSET = maxCurrent;
			
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
			Device.CalibrationFactors.Cal_DDS_FREQ = (CalTmpFrequency == 0 ? 1 : (1000.0f / CalTmpFrequency));		// 1000 = 1000 kHz desired frequency
			
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
	
	grid_Cal.SetRowHeight(1, 100);
	grid_Cal.SetRowHeight(3, 35);
	grid_Cal.AddItem(&ico_Cal, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_Cal.AddItem(&lbl_Cal_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_Cal.AddItem(&lbl_Cal_instruction, 0, 1, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_Cal.AddItem(&numCtrl_Cal_tmpVoltage, 0, 2, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_Cal.AddItem(&numCtrl_Cal_tmpCurrent, 0, 2, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_Cal.AddItem(&numCtrl_Cal_tmpFrequency, 0, 2, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_Cal.AddItem(&lbl_NumCalSteps, 0, 3, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_Cal.AddItem(&progress_Cal, 2, 3, GRID_CELL_ALIGNMENT_LEFT, 3);
	grid_Cal.AddItem(&button_Cal_Next, 0, 4, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_Cal.AddItem(&button_Cal_Exit, 2, 4, GRID_CELL_ALIGNMENT_RIGHT);
	grid_Cal.InitItems();
	
	return &grid_Cal;
}