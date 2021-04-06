/*
 * ScreenDMM.cpp
 *
 * Created: 07.11.2020 13:15:21
 *  Author: V17
 */ 

/*#include "ScreenDMM.h"
#include "../Device.h"
#include "../UserControlsIndicators/UserIndicatorNumeric.cpp"
#include "../UserControlsIndicators/UserIndicatorProgressBar.cpp"
#include "Icons.h"

ScreenDMM::ScreenDMM() : ScreenBase("DMM"),
	_indDMM1(DMM_COLUMN2_POSX, DMM_ROW1_POSY, &Device.DmmChannel1.MeasuredVoltage, "V", 0, 20, 80, 10, PROGRESSBAR_ORIGIN_ZERO, 5),
	_indDMM2(DMM_COLUMN2_POSX, DMM_ROW2_POSY, &Device.DmmChannel2.MeasuredVoltage, "V", 0, 20, 80, 10, PROGRESSBAR_ORIGIN_ZERO, 5)
{
	_userIndicators[0] = &_indDMM1;
	_userIndicators[1] = &_indDMM2;
	_numUserIndicators = 2;
}

void ScreenDMM::Draw(u8g_t* u8g, bool isFirstPage)
{	
	u8g_DrawXBMP(u8g, DMM_COLUMN1_POSX, 5, CONTROLS_ICON_SIZE, CONTROLS_ICON_SIZE, icon_dmm_bits);
	
	u8g_DrawStr(u8g, DMM_COLUMN1_POSX, DMM_ROW1_POSY + INDICATORS_FONT_HEIGHT, "#1:");
	u8g_DrawStr(u8g, DMM_COLUMN1_POSX, DMM_ROW2_POSY + INDICATORS_FONT_HEIGHT, "#2:");

	ScreenBase::Draw(u8g, isFirstPage);
}*/