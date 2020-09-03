/*
 * screen_draw_tabs.h
 *
 * Created: 02.09.2020 19:15:21
 *  Author: V17
 */ 


#ifndef SCREEN_DRAW_TABS_H_
#define SCREEN_DRAW_TABS_H_

#include "../GLCD/u8g.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_TAB_WIDTH		32
#define SCREEN_TAB_HEIGHT		12
#define SCREEN_TAB_MARGIN		1
#define SCREEN_TAB_FONT_HEIGHT	8
#define SCREEN_NUM_TABS			5

void Screen_DrawTabs(u8g_t *u8g, int selectedTabIndex);

#endif /* SCREEN_DRAW_TABS_H_ */