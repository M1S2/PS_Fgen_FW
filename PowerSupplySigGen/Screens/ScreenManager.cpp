/*
 * ScreenManager.cpp
 *
 * Created: 05.11.2020 18:45:05
 *  Author: V17
 */ 

#include <stdio.h>
#include "ScreenManager.h"
#include "../Device.h"

#include "../UI_Lib/UI_Lib_Test.h"

ScreenManagerClass::ScreenManagerClass()
{	
	#ifndef DEVELOPMENT
		_screens[0] = &_screenPs;
		_screens[1] = &_screenDds;
		_screens[2] = NULL;
		_screens[3] = &_screenDmm;
		_screens[4] = &_screenAtx;
	#endif
}

void ScreenManagerClass::Init()
{
	u8g_InitSPI(&_u8g, &u8g_dev_s1d15721_hw_spi, PN(1, 7), PN(1, 5), PN(1, 1), PN(1, 0), U8G_PIN_NONE);
	IsSplashScreenShown = true;
	TimeCounter_SplashScreen_ms = 0;
	
	#ifdef DEVELOPMENT
		UI_Test_Init(&_u8g);
		UI_Test_BuildTree();
	#endif
}

void ScreenManagerClass::drawScreenTabs(int selectedTabIndex)
{	
	selectedTabIndex %= NUM_SCREENS;
	
	u8g_SetFont(&_u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
	u8g_SetDefaultForegroundColor(&_u8g);
	u8g_DrawFrame(&_u8g, SCREEN_TAB_WIDTH - 1, 0, u8g_GetWidth(&_u8g) - SCREEN_TAB_WIDTH + 1, u8g_GetHeight(&_u8g));
	int yTab = 0;
	for(int i=0; i<NUM_SCREENS; i++)
	{
		if(i==selectedTabIndex)
		{
			u8g_DrawFrame(&_u8g, 0, yTab, SCREEN_TAB_WIDTH, SCREEN_TAB_HEIGHT);
			
			u8g_SetDefaultBackgroundColor(&_u8g);
			u8g_DrawVLine(&_u8g, SCREEN_TAB_WIDTH - 1, yTab + 1, SCREEN_TAB_HEIGHT - 2);
			u8g_SetDefaultForegroundColor(&_u8g);

			if(_screens[i] != NULL) { u8g_DrawStr(&_u8g, 2, yTab + SCREEN_TAB_FONT_HEIGHT + ((SCREEN_TAB_HEIGHT - SCREEN_TAB_FONT_HEIGHT) / 2), _screens[i]->TabName); }
		}
		else
		{
			if(_screens[i] != NULL) { u8g_DrawStr(&_u8g, 2, yTab + SCREEN_TAB_FONT_HEIGHT + ((SCREEN_TAB_HEIGHT - SCREEN_TAB_FONT_HEIGHT) / 2), _screens[i]->TabName); }
		}
		yTab+=(SCREEN_TAB_HEIGHT + SCREEN_TAB_MARGIN);
	}
}

void ScreenManagerClass::DrawAll()
{
	bool isFirstPage = true;
	u8g_FirstPage(&_u8g);
	do
	{		
		#ifndef DEVELOPMENT
			drawPage(isFirstPage);
		#else		
			UI_Test_Draw(&_u8g, isFirstPage);
		#endif
		
		isFirstPage = false;
	} while ( u8g_NextPage(&_u8g) );
}

void ScreenManagerClass::drawPage(bool isFirstPage)
{
#ifdef SPLASHSCREEN_ENABLED
	if(IsSplashScreenShown)
	{
		drawSplashScreen();
	}
	else
	{
#endif
		TabIndex %= NUM_SCREENS;
		drawScreenTabs(TabIndex);
		for(int i=0; i<NUM_SCREENS; i++)
		{
			if(i != TabIndex || _screens[i] == NULL) { continue; }
			_screens[i]->Draw(&_u8g, isFirstPage);
		}
	
		drawStatusBar();
		drawMessage();
#ifdef SPLASHSCREEN_ENABLED
	}
#endif
}

void ScreenManagerClass::drawStatusBar()
{
	u8g_DrawFrame(&_u8g, u8g_GetWidth(&_u8g) - 32, 0, 32, 12);

	if(Device.DevSettingsNeedSaving)
	{
		u8g_SetFont(&_u8g, u8g_font_7x14r);		// 10 pixel height font
		u8g_DrawStr(&_u8g, u8g_GetWidth(&_u8g) - 8, 10, "*");
	}
			
	u8g_SetFont(&_u8g, u8g_font_helvR08r);	// 8 pixel height font, 6 pixel width
	switch(Device.DeviceControlState)
	{
		case DEV_CTRL_LOCAL: u8g_DrawStr(&_u8g, u8g_GetWidth(&_u8g) - 30, 10, "LOC"); break;
		case DEV_CTRL_REMOTE: u8g_DrawStr(&_u8g, u8g_GetWidth(&_u8g) - 30, 10, "REM"); break;
		case DEV_CTRL_RWLOCK: u8g_DrawStr(&_u8g, u8g_GetWidth(&_u8g) - 30, 10, "RWL"); break;
	}
}

void ScreenManagerClass::drawMessage()
{
	if((SystemMessage != NULL && strcmp(SystemMessage, "") != 0) || (UserMessage != NULL && strcmp(UserMessage, "") != 0))
	{
		u8g_SetDefaultBackgroundColor(&_u8g);
		u8g_DrawBox(&_u8g, MESSAGE_MARGIN, MESSAGE_MARGIN, u8g_GetWidth(&_u8g) - 2 * MESSAGE_MARGIN, u8g_GetHeight(&_u8g) - 2 * MESSAGE_MARGIN);
		u8g_SetDefaultForegroundColor(&_u8g);
		u8g_DrawFrame(&_u8g, MESSAGE_MARGIN, MESSAGE_MARGIN, u8g_GetWidth(&_u8g) - 2 * MESSAGE_MARGIN, u8g_GetHeight(&_u8g) - 2 * MESSAGE_MARGIN);
	}
	
	if(SystemMessage != NULL && strcmp(SystemMessage, "") != 0)
	{
		u8g_DrawStrMultiline(&_u8g, MESSAGE_MARGIN + 2, MESSAGE_MARGIN + 2 + 8, SystemMessage);
	}
	else if(UserMessage != NULL && strcmp(UserMessage, "") != 0)
	{
		u8g_DrawStrMultiline(&_u8g, MESSAGE_MARGIN + 2, MESSAGE_MARGIN + 2 + 8, UserMessage);
	}
}

void ScreenManagerClass::DeviceTimerTickISR(uint16_t currentPeriod_ms)
{
	#ifdef SPLASHSCREEN_ENABLED
		if(IsSplashScreenShown) { TimeCounter_SplashScreen_ms += currentPeriod_ms; }
			
		/* Hide splash screen after some time */
		if(IsSplashScreenShown && (TimeCounter_SplashScreen_ms >= SPLASHSCREEN_DELAY_MS))
		{
			IsSplashScreenShown = false;
		}
	#endif
}

void ScreenManagerClass::KeyInput(Keys_t key)
{
	UI_Test_KeyInput(key);
	
	if(key == KEYOK)
	{
		if(_screens[TabIndex] != NULL)
		{
			_isControlActive = !_isControlActive;
			_screens[TabIndex]->ActivateSelectedControl(_isControlActive);
			_screens[TabIndex]->EncoderPBInput();
			_isControlActive = _screens[TabIndex]->IsSelectedControlActive();
		}
	}
	else if(key == KEYDOWN || key == KEYUP)
	{
		if(_screens[TabIndex] != NULL)
		{
			if(key == KEYDOWN) { _screens[TabIndex]->EncoderInput(ENCCLOCKWISE, _isControlActive); }
			else { _screens[TabIndex]->EncoderInput(ENCCOUNTERCLOCKWISE, _isControlActive); }
			_isControlActive = _screens[TabIndex]->IsSelectedControlActive();
		}
	}
	else
	{
		if(_isControlActive)
		{
			if(_screens[TabIndex] != NULL)
			{
				_screens[TabIndex]->KeyInput(key);
				_isControlActive = _screens[TabIndex]->IsSelectedControlActive();
			}
		}
		else
		{
			if(key == KEYLEFT) 
			{
				 TabIndex = (TabIndex - 1 < 0 ? NUM_SCREENS - 1 : TabIndex - 1); 
			}
			else if(key == KEYRIGHT) 
			{
				 TabIndex = (TabIndex + 1 == NUM_SCREENS ? 0: TabIndex + 1); 
			}
			Device.SaveSettings();
		}
	}
}

void ScreenManagerClass::SetDisplayEnabled(bool displayEnabled)
{
	_displayEnabled = displayEnabled;
	if(displayEnabled) { u8g_SleepOff(&_u8g); }
	else { u8g_SleepOn(&_u8g); }
}

bool ScreenManagerClass::GetDisplayEnabled()
{
	return _displayEnabled;
}

void ScreenManagerClass::SetDisplayInverted(bool displayInverse)
{
	Device.DevSettingsNeedSaving = (DisplayInverted != displayInverse);
	DisplayInverted = displayInverse;
	u8g_Invert(&_u8g, (uint8_t)displayInverse);
}