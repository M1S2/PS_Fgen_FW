/*
 * ScreenManager.cpp
 *
 * Created: 05.11.2020 18:45:05
 *  Author: V17
 */ 

#include "ScreenManager.h"
#include "../USART/USART.h"
#include "ScreenPS.h"
#include "ScreenDMM.h"
#include "ScreenATX.h"

ScreenManagerClass ScreenManager;

ScreenPS screenPs;
ScreenDMM screenDmm;
ScreenATX screenAtx;

ScreenManagerClass::ScreenManagerClass()
{	
	_screens[0] = &screenPs;
	_screens[1] = NULL;
	_screens[2] = NULL;
	_screens[3] = &screenDmm;
	_screens[4] = &screenAtx;
}

void ScreenManagerClass::drawScreenTabs(int selectedTabIndex)
{	
	selectedTabIndex %= NUM_SCREENS;
	
	u8g_SetFont(_u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
	u8g_SetDefaultForegroundColor(_u8g);
	int yTab = 0;
	for(int i=0; i<NUM_SCREENS; i++)
	{
		if(i==selectedTabIndex)
		{
			u8g_DrawBox(_u8g, 0, yTab, SCREEN_TAB_WIDTH, SCREEN_TAB_HEIGHT);
			u8g_SetDefaultBackgroundColor(_u8g);
			u8g_DrawStr(_u8g, 2, yTab + SCREEN_TAB_FONT_HEIGHT + ((SCREEN_TAB_HEIGHT - SCREEN_TAB_FONT_HEIGHT) / 2), _screens[i]->TabName);
			u8g_SetDefaultForegroundColor(_u8g);
		}
		else
		{
			u8g_DrawFrame(_u8g, 0, yTab, SCREEN_TAB_WIDTH, SCREEN_TAB_HEIGHT);
			u8g_DrawStr(_u8g, 2, yTab + SCREEN_TAB_FONT_HEIGHT + ((SCREEN_TAB_HEIGHT - SCREEN_TAB_FONT_HEIGHT) / 2), _screens[i]->TabName);
		}
		yTab+=(SCREEN_TAB_HEIGHT + SCREEN_TAB_MARGIN);
	}
	u8g_DrawFrame(_u8g, SCREEN_TAB_WIDTH - 1, 0, u8g_GetWidth(_u8g) - SCREEN_TAB_WIDTH + 1, u8g_GetHeight(_u8g));
}

void ScreenManagerClass::Draw(DevStatus_t devStatusDraw, DevSettings_t devSettingsDraw)
{
	drawScreenTabs(devSettingsDraw.TabIndex);
	for(int i=0; i<NUM_SCREENS; i++)
	{
		if(i != devSettingsDraw.TabIndex || _screens[i] == NULL) { continue; }
		_screens[i]->Draw(_u8g, devStatusDraw);
	}
}