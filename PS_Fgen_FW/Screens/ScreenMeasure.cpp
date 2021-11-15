/*
 * ScreenMeasure.cpp
 * Created: 06.04.2021 19:45:48
 * Author: Markus Scheich
 */ 

#include "../Device.h"

#ifdef MEASURE_SUBSYSTEM_ENABLED

ContainerList list_Measure(40, 0, 240 - 40, 64);

// ***** Measure DMM page *****
#define DMM_COLUMN1_POSX		SCREEN_TAB_WIDTH + 5
#define DMM_COLUMN2_POSX		DMM_COLUMN1_POSX + 15
#define DMM_COLUMN3_POSX		DMM_COLUMN2_POSX + 75
#define DMM_ROW1_POSY			25
#define DMM_ROW2_POSY			DMM_ROW1_POSY + 25

ContainerPage page_DMM;
Icon ico_dmm(SCREEN_TAB_WIDTH + 5, 3, icon_dmm_width, icon_dmm_height, icon_dmm_bits);
Label<5> lbl_DMM_caption(SCREEN_TAB_WIDTH + 25, 5, "DMM");
Label<5> lbl_DMM_No1(DMM_COLUMN1_POSX, DMM_ROW1_POSY, "#1:");
NumericIndicator<volatile float> numInd_DMM1(DMM_COLUMN2_POSX, DMM_ROW1_POSY, &Device.DmmChannel1.MeasuredVoltage, "V", 20, 3);
ProgressBar<volatile float> progress_DMM1(DMM_COLUMN3_POSX, DMM_ROW1_POSY, 90, 10, &Device.DmmChannel1.MeasuredVoltage, 0, 20, PROGRESSBAR_ORIGIN_ZERO, 5);
Label<5> lbl_DMM_No2(DMM_COLUMN1_POSX, DMM_ROW2_POSY, "#2:");
NumericIndicator<volatile float> numInd_DMM2(DMM_COLUMN2_POSX, DMM_ROW2_POSY, &Device.DmmChannel2.MeasuredVoltage, "V", 20, 3);
ProgressBar<volatile float> progress_DMM2(DMM_COLUMN3_POSX, DMM_ROW2_POSY, 90, 10, &Device.DmmChannel2.MeasuredVoltage, 0, 20, PROGRESSBAR_ORIGIN_ZERO, 5);

// ***** Measure ATX page *****
#define ATX_COLUMN1_POSX		SCREEN_TAB_WIDTH + 5
#define ATX_COLUMN2_POSX		ATX_COLUMN1_POSX + 35
#define ATX_COLUMN3_POSX		ATX_COLUMN2_POSX + 65
#define ATX_ROW1_POSY			21
#define ATX_ROW2_POSY			ATX_ROW1_POSY + 10
#define ATX_ROW3_POSY			ATX_ROW2_POSY + 10
#define ATX_ROW4_POSY			ATX_ROW3_POSY + 10

ContainerPage page_ATX;
Icon ico_atx(SCREEN_TAB_WIDTH + 5, 3, icon_voltage_width, icon_voltage_height, icon_voltage_bits);
Label<5> lbl_ATX_caption(SCREEN_TAB_WIDTH + 25, 5, "ATX");
Label<6> lbl_ATX_3V3(ATX_COLUMN1_POSX, ATX_ROW1_POSY, "3.3V:");
Label<5> lbl_ATX_5V(ATX_COLUMN1_POSX, ATX_ROW2_POSY, "5V:");
Label<5> lbl_ATX_12V(ATX_COLUMN1_POSX, ATX_ROW3_POSY, "12V:");
Label<6> lbl_ATX_12V_NEG(ATX_COLUMN1_POSX, ATX_ROW4_POSY, "-12V:");
NumericIndicator<float> numInd_ATX_3V3(ATX_COLUMN2_POSX, ATX_ROW1_POSY, &Device.DeviceVoltages.ATX_3V3, "V", 4, 2);
ProgressBar<float> progress_ATX_3V3(ATX_COLUMN3_POSX, ATX_ROW1_POSY, 80, 5, &Device.DeviceVoltages.ATX_3V3, 0, 4, PROGRESSBAR_ORIGIN_ZERO, 1);
NumericIndicator<float> numInd_ATX_5V(ATX_COLUMN2_POSX, ATX_ROW2_POSY, &Device.DeviceVoltages.ATX_5V, "V", 6, 2);
ProgressBar<float> progress_ATX_5V(ATX_COLUMN3_POSX, ATX_ROW2_POSY, 80, 5, &Device.DeviceVoltages.ATX_5V, 0, 6, PROGRESSBAR_ORIGIN_ZERO, 1);
NumericIndicator<float> numInd_ATX_12V(ATX_COLUMN2_POSX, ATX_ROW3_POSY, &Device.DeviceVoltages.ATX_12V, "V", 15, 2);
ProgressBar<float> progress_ATX_12V(ATX_COLUMN3_POSX, ATX_ROW3_POSY, 80, 5, &Device.DeviceVoltages.ATX_12V, 0, 15, PROGRESSBAR_ORIGIN_ZERO, 1);
NumericIndicator<float> numInd_ATX_12V_NEG(ATX_COLUMN2_POSX, ATX_ROW4_POSY, &Device.DeviceVoltages.ATX_12V_NEG, "V", -15, 2);
ProgressBar<float> progress_ATX_12V_NEG(ATX_COLUMN3_POSX, ATX_ROW4_POSY, 80, 5, &Device.DeviceVoltages.ATX_12V_NEG, -15, 0, PROGRESSBAR_ORIGIN_ZERO, 1);


UIElement* uiBuildScreenMeasure()
{
	page_DMM.AddItem(&ico_dmm);
	page_DMM.AddItem(&lbl_DMM_caption);
	page_DMM.AddItem(&lbl_DMM_No1);
	page_DMM.AddItem(&numInd_DMM1);
	page_DMM.AddItem(&progress_DMM1);
	page_DMM.AddItem(&lbl_DMM_No2);
	page_DMM.AddItem(&numInd_DMM2);
	page_DMM.AddItem(&progress_DMM2);
	page_DMM.InitItems();
	
	page_ATX.AddItem(&ico_atx);
	page_ATX.AddItem(&lbl_ATX_caption);
	page_ATX.AddItem(&lbl_ATX_3V3);
	page_ATX.AddItem(&numInd_ATX_3V3);
	page_ATX.AddItem(&progress_ATX_3V3);
	page_ATX.AddItem(&lbl_ATX_5V);
	page_ATX.AddItem(&numInd_ATX_5V);
	page_ATX.AddItem(&progress_ATX_5V);
	page_ATX.AddItem(&lbl_ATX_12V);
	page_ATX.AddItem(&numInd_ATX_12V);
	page_ATX.AddItem(&progress_ATX_12V);
	page_ATX.AddItem(&lbl_ATX_12V_NEG);
	page_ATX.AddItem(&numInd_ATX_12V_NEG);
	page_ATX.AddItem(&progress_ATX_12V_NEG);
	page_ATX.InitItems();
	
	list_Measure.AddItem(&page_DMM);
	list_Measure.AddItem(&page_ATX);
	
	return &list_Measure;
}

#endif /* MEASURE_SUBSYSTEM_ENABLED */