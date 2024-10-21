/*
 * ScreenMeasure.cpp
 * Created: 06.04.2021 19:45:48
 * Author: Markus Scheich
 */ 

#include "../Device.h"

#ifdef MEASURE_SUBSYSTEM_ENABLED

ContainerListDefault list_Measure;

// ***** Measure DMM page *****
ContainerGridDefault grid_DMM;
Icon ico_dmm(icon_dmm_width, icon_dmm_height, icon_dmm_bits);
Label<5> lbl_DMM_caption("DMM");
Label<5> lbl_DMM_No1("#1:");
NumericIndicator<volatile float> numInd_DMM1(&Device.DmmChannel1.MeasuredVoltage, "V", 20, 3);
Icon ico_dmm1_neg_warning(icon_warning_width, icon_warning_height, icon_warning_bits);
ProgressBar<volatile float> progress_DMM1(&Device.DmmChannel1.MeasuredVoltage, 0, 20, PROGRESSBAR_ORIGIN_ZERO, 5, 70, 10);
Label<5> lbl_DMM_No2("#2:");
NumericIndicator<volatile float> numInd_DMM2(&Device.DmmChannel2.MeasuredVoltage, "V", 20, 3);
Icon ico_dmm2_neg_warning(icon_warning_width, icon_warning_height, icon_warning_bits);
ProgressBar<volatile float> progress_DMM2(&Device.DmmChannel2.MeasuredVoltage, 0, 20, PROGRESSBAR_ORIGIN_ZERO, 5, 70, 10);

// ***** Measure ATX page *****
ContainerGridDefault grid_ATX;
Icon ico_atx(icon_voltage_width, icon_voltage_height, icon_voltage_bits);
Label<5> lbl_ATX_caption("ATX");
Label<6> lbl_ATX_3V3("3.3V:");
Label<5> lbl_ATX_5V("5V:");
Label<5> lbl_ATX_12V("12V:");
Label<6> lbl_ATX_12V_NEG("-12V:");
NumericIndicator<float> numInd_ATX_3V3(&Device.DeviceVoltages.ATX_3V3, "V", 4, 2);
ProgressBar<float> progress_ATX_3V3(&Device.DeviceVoltages.ATX_3V3, 0, 4, PROGRESSBAR_ORIGIN_ZERO, 1, 65, 5);
NumericIndicator<float> numInd_ATX_5V(&Device.DeviceVoltages.ATX_5V, "V", 6, 2);
ProgressBar<float> progress_ATX_5V(&Device.DeviceVoltages.ATX_5V, 0, 6, PROGRESSBAR_ORIGIN_ZERO, 1, 65, 5);
NumericIndicator<float> numInd_ATX_12V(&Device.DeviceVoltages.ATX_12V, "V", 15, 2);
ProgressBar<float> progress_ATX_12V(&Device.DeviceVoltages.ATX_12V, 0, 15, PROGRESSBAR_ORIGIN_ZERO, 1, 65, 5);
NumericIndicator<float> numInd_ATX_12V_NEG(&Device.DeviceVoltages.ATX_12V_NEG, "V", -15, 2);
ProgressBar<float> progress_ATX_12V_NEG(&Device.DeviceVoltages.ATX_12V_NEG, -15, 0, PROGRESSBAR_ORIGIN_ZERO, 1, 65, 5);

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

	grid_DMM.SetColumnWidth(0, 25);
	grid_DMM.AddItem(&ico_dmm, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&lbl_DMM_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&lbl_DMM_No1, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&numInd_DMM1, 1, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&ico_dmm1_neg_warning, 2, 1, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_DMM.AddItem(&progress_DMM1, 3, 1, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_DMM.AddItem(&lbl_DMM_No2, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&numInd_DMM2, 1, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_DMM.AddItem(&ico_dmm2_neg_warning, 2, 2, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_DMM.AddItem(&progress_DMM2, 3, 2, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_DMM.InitItems();
	
	grid_ATX.SetColumnWidth(0, 25);
	grid_ATX.AddItem(&ico_atx, 0, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&lbl_ATX_caption, 1, 0, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&lbl_ATX_3V3, 0, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&numInd_ATX_3V3, 2, 1, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&progress_ATX_3V3, 3, 1, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_ATX.AddItem(&lbl_ATX_5V, 0, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&numInd_ATX_5V, 2, 2, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&progress_ATX_5V, 3, 2, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_ATX.AddItem(&lbl_ATX_12V, 0, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&numInd_ATX_12V, 2, 3, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&progress_ATX_12V, 3, 3, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_ATX.AddItem(&lbl_ATX_12V_NEG, 0, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&numInd_ATX_12V_NEG, 2, 4, GRID_CELL_ALIGNMENT_LEFT);
	grid_ATX.AddItem(&progress_ATX_12V_NEG, 3, 4, GRID_CELL_ALIGNMENT_MIDDLE);
	grid_ATX.InitItems();
	
	return &list_Measure;
}

#endif /* MEASURE_SUBSYSTEM_ENABLED */