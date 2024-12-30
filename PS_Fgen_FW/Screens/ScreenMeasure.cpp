/*
 * ScreenMeasure.cpp
 * Created: 06.04.2021 19:45:48
 * Author: Markus Scheich
 */ 

#include "../Device.h"

#ifdef MEASURE_SUBSYSTEM_ENABLED

ContainerList list_Measure(2, 20);

// ***** Measure DMM page *****
ContainerGrid grid_DMM(10, 5, 3, false, true);
Icon ico_dmm(icon_dmm_width, icon_dmm_height, icon_dmm_bits, COLOR_FOREGROUND_HEADERS);
Label lbl_DMM_caption("DMM", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 5);
Label lbl_DMM_No1("#1:", LABEL_COLOR_NOTSET, NULL, 0, 0, 5);
NumericIndicator<volatile float> numInd_DMM1(&Device.DmmChannel1.MeasuredVoltage, "V", 20, 3);
Icon ico_dmm1_neg_warning(icon_warning_width, icon_warning_height, icon_warning_bits);
ProgressBar<volatile float> progress_DMM1(&Device.DmmChannel1.MeasuredVoltage, 0, 20, PROGRESSBAR_ORIGIN_ZERO, 5, 150, 20);
Label lbl_DMM_No2("#2:", LABEL_COLOR_NOTSET, NULL, 0, 0, 5);
NumericIndicator<volatile float> numInd_DMM2(&Device.DmmChannel2.MeasuredVoltage, "V", 20, 3);
Icon ico_dmm2_neg_warning(icon_warning_width, icon_warning_height, icon_warning_bits);
ProgressBar<volatile float> progress_DMM2(&Device.DmmChannel2.MeasuredVoltage, 0, 20, PROGRESSBAR_ORIGIN_ZERO, 5, 150, 20);

// ***** Measure ATX page *****
ContainerGrid grid_ATX(6, 5, 4, false, true);
Icon ico_atx(icon_voltage_width, icon_voltage_height, icon_voltage_bits, COLOR_FOREGROUND_HEADERS);
Label lbl_ATX_caption("ATX", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 5);
NumericIndicator<float> numInd_ATX_3V3(&Device.DeviceVoltages.ATX_3V3, "V", 4, 2);
ProgressBar<float> progress_ATX_3V3(&Device.DeviceVoltages.ATX_3V3, 0, 4, PROGRESSBAR_ORIGIN_ZERO, 1, 170, 20);
NumericIndicator<float> numInd_ATX_5V(&Device.DeviceVoltages.ATX_5V, "V", 6, 2);
ProgressBar<float> progress_ATX_5V(&Device.DeviceVoltages.ATX_5V, 0, 6, PROGRESSBAR_ORIGIN_ZERO, 1, 170, 20);

/** 
 * Update the visibility of the DMM channel negative warning icons.
 */
void updateDmmNegativeWarningVisibility()
{
	ico_dmm1_neg_warning.Visible = Device.DmmChannel1.IsNegative;
	ico_dmm2_neg_warning.Visible = Device.DmmChannel2.IsNegative;
}

UIElement* uiBuildScreenMeasure()
{
	ico_dmm1_neg_warning.Visible = false;
	ico_dmm2_neg_warning.Visible = false;
	
	list_Measure.AddItem(&grid_DMM);
	list_Measure.AddItem(&grid_ATX);

	grid_DMM.SetRowHeight(2, 60);		// Add some space between both DMM indicators
	grid_DMM.AddItem(&ico_dmm, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&lbl_DMM_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&lbl_DMM_No1, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&numInd_DMM1, 1, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&ico_dmm1_neg_warning, 2, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&progress_DMM1, 1, 2, GRID_CELL_ALIGNMENT_TOP_LEFT, 2);
	grid_DMM.AddItem(&lbl_DMM_No2, 0, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&numInd_DMM2, 1, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&ico_dmm2_neg_warning, 2, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&progress_DMM2, 1, 4, GRID_CELL_ALIGNMENT_TOP_LEFT, 2);
	grid_DMM.InitItems();
	
	grid_ATX.SetRowHeight(2, 50);		// Add some space between both ATX indicators
	grid_ATX.AddItem(&ico_atx, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&lbl_ATX_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT, 2);
	grid_ATX.AddItem(&numInd_ATX_3V3, 1, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&progress_ATX_3V3, 1, 2, GRID_CELL_ALIGNMENT_TOP_LEFT);
	grid_ATX.AddItem(&numInd_ATX_5V, 1, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&progress_ATX_5V, 1, 4, GRID_CELL_ALIGNMENT_TOP_LEFT);
	grid_ATX.InitItems();
	
	return &list_Measure;
}

#endif /* MEASURE_SUBSYSTEM_ENABLED */