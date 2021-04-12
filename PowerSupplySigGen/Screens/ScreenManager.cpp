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

ContainerPage page_Main;
EnumIndicator<DeviceControlStates_t> enumInd_deviceState(240 - 37, 2, &Device.DeviceControlState, DeviceControlStateNames, 3);
Label lbl_devSettingsNeedSaving(240 - 15, 0, "*", u8g_font_7x14r);

TabControl tabControlMain(0, 0, 240, 64, SCREEN_TAB_WIDTH);


ScreenManagerClass::ScreenManagerClass()
{	
}

void ScreenManagerClass::Init()
{
	u8g_InitSPI(&_u8g, &u8g_dev_s1d15721_hw_spi, PN(1, 7), PN(1, 5), PN(1, 1), PN(1, 0), U8G_PIN_NONE);
	IsSplashScreenShown = true;
	TimeCounter_SplashScreen_ms = 0;
	
	_uiManager.Init(&_u8g);
	uiBuildTree();
		
	//UI_Test_Init(&_u8g);
	//UI_Test_BuildTree();
}

void ScreenManagerClass::uiBuildTree()
{
	tabControlMain.AddTab("PS", uiBuildScreenPS());	
	tabControlMain.AddTab("DDS", uiBuildScreenDDS());			// Containing DDS1 and DDS2
	tabControlMain.AddTab("Meas", uiBuildScreenMeasure());		// Containing DMM and ATX measurements
	tabControlMain.AddTab("Conf", uiBuildScreenSettings());
	tabControlMain.SelectTab(0);
	
	page_Main.AddItem(&tabControlMain);
	page_Main.AddItem(&enumInd_deviceState);
	page_Main.AddItem(&lbl_devSettingsNeedSaving);
	page_Main.InitItems();
	
	#ifdef SPLASHSCREEN_ENABLED 
		_uiManager.ChangeVisualTreeRoot(uiBuildSplashScreen());
	#else
		_uiManager.ChangeVisualTreeRoot(&page_Main);
	#endif
}

void ScreenManagerClass::UpdateSettingsChangedIndicator(bool settingsChanged)
{
	lbl_devSettingsNeedSaving.Visible = settingsChanged;
}

void ScreenManagerClass::DrawAll()
{
	bool isFirstPage = true;
	u8g_FirstPage(&_u8g);
	do
	{		
		_uiManager.Draw(&_u8g, isFirstPage);	
		//UI_Test_Draw(&_u8g, isFirstPage);
		
		isFirstPage = false;
	} while ( u8g_NextPage(&_u8g) );
}

void ScreenManagerClass::drawPage(bool isFirstPage)
{
	drawStatusBar();
	drawMessage();
}

void ScreenManagerClass::drawStatusBar()
{
	u8g_DrawFrame(&_u8g, u8g_GetWidth(&_u8g) - 32, 0, 32, 12);
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
			_uiManager.ChangeVisualTreeRoot(&page_Main);
		}
	#endif
}

void ScreenManagerClass::KeyInput(Keys_t key)
{
	_uiManager.KeyInput(key);
	//UI_Test_KeyInput(key);
	
	//Device.SaveSettings();
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
	Device.SetSettingsChanged(DisplayInverted != displayInverse);
	DisplayInverted = displayInverse;
	u8g_Invert(&_u8g, (uint8_t)displayInverse);
}