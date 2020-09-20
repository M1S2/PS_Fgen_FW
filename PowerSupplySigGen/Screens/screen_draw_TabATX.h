/*
 * screen_draw_TabATX.h
 *
 * Created: 19.09.2020 19:09:33
 *  Author: V17
 */ 


#ifndef SCREEN_DRAW_TABATX_H_
#define SCREEN_DRAW_TABATX_H_

#include "screen_draw_tabs.h"
#include "../Global/DevStatus.h"

#define ATX_COLUMN1_POSX		SCREEN_TAB_WIDTH + 10
#define ATX_COLUMN2_POSX		ATX_COLUMN1_POSX + 35
#define ATX_COLUMN3_POSX		ATX_COLUMN2_POSX + 70
#define ATX_COLUMN4_POSX		ATX_COLUMN3_POSX + 35
#define ATX_ROW1_POSY			u8g_GetHeight(u8g) / 2 - 7
#define ATX_ROW2_POSY			u8g_GetHeight(u8g) / 2 + 10 + 7

void Screen_DrawTabATX(u8g_t *u8g, DevStatus_t devStatusDraw);

#endif /* SCREEN_DRAW_TABATX_H_ */