/*
 * ScreenATX.cpp
 *
 * Created: 07.11.2020 13:23:55
 *  Author: V17
 */ 

/*#include "ScreenATX.h"
#include "../Device.h"
#include "../UserControlsIndicators/UserIndicatorNumeric.cpp"
#include "../UserControlsIndicators/UserIndicatorProgressBar.cpp"

ScreenATX::ScreenATX() : ScreenBase("ATX"),
	_indATX3V3(ATX_COLUMN2_POSX, ATX_ROW1_POSY, &Device.DeviceVoltages.ATX_3V3, "V", 0, 5, 75, 5, PROGRESSBAR_ORIGIN_ZERO, 1),
	_indATX5V(ATX_COLUMN2_POSX, ATX_ROW2_POSY, &Device.DeviceVoltages.ATX_5V, "V", 0, 6, 75, 5, PROGRESSBAR_ORIGIN_ZERO, 1),
	_indATX12V(ATX_COLUMN2_POSX, ATX_ROW3_POSY, &Device.DeviceVoltages.ATX_12V, "V", 0, 15, 75, 5, PROGRESSBAR_ORIGIN_ZERO, 1),
	_indATX12VNEG(ATX_COLUMN2_POSX, ATX_ROW4_POSY, &Device.DeviceVoltages.ATX_12V_NEG, "V", -15, 0, 75, 5, PROGRESSBAR_ORIGIN_ZERO, 1)
{
	_userIndicators[0] = &_indATX3V3;
	_userIndicators[1] = &_indATX5V;
	_userIndicators[2] = &_indATX12V;
	_userIndicators[3] = &_indATX12VNEG;
	_numUserIndicators = 4;
}

void ScreenATX::Draw(u8g_t* u8g, bool isFirstPage)
{
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW1_POSY + INDICATORS_FONT_HEIGHT, "3V3:");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW2_POSY + INDICATORS_FONT_HEIGHT, "5V:");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW3_POSY + INDICATORS_FONT_HEIGHT, "12V:");
	u8g_DrawStr(u8g, ATX_COLUMN1_POSX, ATX_ROW4_POSY + INDICATORS_FONT_HEIGHT, "-12V:");

	ScreenBase::Draw(u8g, isFirstPage);
}*/