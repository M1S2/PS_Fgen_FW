/*
 * screen_draw_TabPS.h
 *
 * Created: 02.09.2020 19:53:02
 *  Author: V17
 */ 


#ifndef SCREEN_DRAW_TABPS_H_
#define SCREEN_DRAW_TABPS_H_

#include "screen_draw_tabs.h"
#include "../Global/DevStatus.h"
#include "../Global/DevSettings.h"

#define CONTROLS_FONT_HEIGHT		10														// 10 pixel height font
#define VOLTAGE_CONTROL_POSX		SCREEN_TAB_WIDTH + 10
#define VOLTAGE_CONTROL_POSY		u8g_GetHeight(u8g) / 2 - 7
#define OUTPUT_STATE_CONTROL_POSX	VOLTAGE_CONTROL_POSX
#define OUTPUT_STATE_CONTROL_POSY	u8g_GetHeight(u8g) / 2 + CONTROLS_FONT_HEIGHT + 7
#define INFO_TEXTS_FONT_HEIGHT		10														// 10 pixel height font
#define INFO_TEXTS_POSX				VOLTAGE_CONTROL_POSX + 7 * 7 + 60						// voltage control uses 7 pixel width font
#define INFO_TEXT_VOLTAGE_POSY		u8g_GetHeight(u8g) / 2 + INFO_TEXTS_FONT_HEIGHT / 2
#define INFO_TEXT_CURRENT_POSY		INFO_TEXT_VOLTAGE_POSY - INFO_TEXTS_FONT_HEIGHT - 5
#define INFO_TEXT_POWER_POSY		INFO_TEXT_VOLTAGE_POSY + INFO_TEXTS_FONT_HEIGHT + 5

void Screen_DrawTabPS(u8g_t *u8g, DevStatus_t devStatusDraw, DevSettings_t devSettingsDraw);

#endif /* SCREEN_DRAW_TABPS_H_ */