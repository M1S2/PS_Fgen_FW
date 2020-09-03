/*
 * screen_draw_tabs.cpp
 *
 * Created: 02.09.2020 19:15:29
 *  Author: V17
 */ 

#include "screen_draw_tabs.h"

const char* ScreenTabNames[] = {"PS", "DDS1", "DDS2", "DMM", "ATX"};

/*Draw the basic tab structure with PS tab selected*/
void Screen_DrawTabs(u8g_t *u8g, int selectedTabIndex)
{
	selectedTabIndex %= SCREEN_NUM_TABS;
	
	u8g_SetFont(u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
	u8g_SetDefaultForegroundColor(u8g);
	int yTab = 0;
	for(int i=0; i<SCREEN_NUM_TABS; i++)
	{
		if(i==selectedTabIndex)
		{
			u8g_DrawBox(u8g, 0, yTab, SCREEN_TAB_WIDTH, SCREEN_TAB_HEIGHT);
			u8g_SetDefaultBackgroundColor(u8g);
			u8g_DrawStr(u8g, 2, yTab + SCREEN_TAB_FONT_HEIGHT + ((SCREEN_TAB_HEIGHT - SCREEN_TAB_FONT_HEIGHT) / 2), ScreenTabNames[i]);
			u8g_SetDefaultForegroundColor(u8g);
		}
		else
		{
			u8g_DrawFrame(u8g, 0, yTab, SCREEN_TAB_WIDTH, SCREEN_TAB_HEIGHT);
			u8g_DrawStr(u8g, 2, yTab + SCREEN_TAB_FONT_HEIGHT + ((SCREEN_TAB_HEIGHT - SCREEN_TAB_FONT_HEIGHT) / 2), ScreenTabNames[i]);
		}
		yTab+=(SCREEN_TAB_HEIGHT + SCREEN_TAB_MARGIN);
	}
	u8g_DrawFrame(u8g, SCREEN_TAB_WIDTH - 1, 0, u8g_GetWidth(u8g) - SCREEN_TAB_WIDTH + 1, u8g_GetHeight(u8g));
}