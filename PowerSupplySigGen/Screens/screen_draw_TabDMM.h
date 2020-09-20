/*
 * screen_draw_TabDMM.h
 *
 * Created: 20.09.2020 12:33:39
 *  Author: V17
 */ 


#ifndef SCREEN_DRAW_TABDMM_H_
#define SCREEN_DRAW_TABDMM_H_

#include "screen_draw_tabs.h"
#include "../Global/DevStatus.h"

#define DMM_COLUMN1_POSX		SCREEN_TAB_WIDTH + 10
#define DMM_COLUMN2_POSX		DMM_COLUMN1_POSX + 50
#define DMM_ROW1_POSY			u8g_GetHeight(u8g) / 2 - 7
#define DMM_ROW2_POSY			u8g_GetHeight(u8g) / 2 + 10 + 7

void Screen_DrawTabDMM(u8g_t *u8g, DevStatus_t devStatusDraw);


#endif /* SCREEN_DRAW_TABDMM_H_ */