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

void TabControlTabChanged(void* context);

ContainerPage page_Main;
EnumIndicator<DeviceControlStates_t> enumInd_deviceState(240 - 37, 2, &Device.DeviceControlState, DeviceControlStateNames, NUM_DEV_CTRL_ELEMENTS);
Label<5> lbl_devSettingsNeedSaving(240 - 15, 0, "*", u8g_font_7x14r);

TabControl tabControlMain(0, 0, 240, 64, SCREEN_TAB_WIDTH);

MessageDialog msg_DeviceRWLState(0, 0, 240, 64, "Device locked by SYST:RWL command.\nUnlock with SYST:LOC command.", MSG_WARNING, MSG_BTN_NONE);

ScreenManagerClass::ScreenManagerClass()
{	
}

void ScreenManagerClass::Init()
{
	u8g_InitSPI(&_u8g, &u8g_dev_s1d15721_hw_spi, PN(1, 7), PN(1, 5), PN(1, 1), PN(1, 0), U8G_PIN_NONE);
	IsSplashScreenShown = true;
	TimeCounter_SplashScreen_ms = 0;
	
	UiManager.Init(&_u8g);
	uiBuildTree();
}

void ScreenManagerClass::uiBuildTree()
{
	tabControlMain.AddTab("PS", uiBuildScreenPS());	
	tabControlMain.AddTab("DDS", uiBuildScreenDDS());			// Containing DDS1 and DDS2
	tabControlMain.AddTab("Meas", uiBuildScreenMeasure());		// Containing DMM and ATX measurements
	tabControlMain.AddTab("Conf", uiBuildScreenSettings());
	
	page_Main.AddItem(&tabControlMain);
	page_Main.AddItem(&enumInd_deviceState);
	page_Main.AddItem(&lbl_devSettingsNeedSaving);
	page_Main.InitItems();
	
	uiBuildScreenCalibration();
	
	#ifdef SPLASHSCREEN_ENABLED 
		UiManager.ChangeVisualTreeRoot(uiBuildSplashScreen());
	#else
		UiManager.ChangeVisualTreeRoot(&page_Main);
	#endif
}

void ScreenManagerClass::ShowUiMainPage()
{
	UiManager.ChangeVisualTreeRoot(&page_Main);
}

void ScreenManagerClass::ShowUiCalibrationMenu()
{
	UiManager.ChangeVisualTreeRoot(StartCalibration());
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
		UiManager.Draw(&_u8g, isFirstPage);			
		isFirstPage = false;
	} while ( u8g_NextPage(&_u8g) );
}

void ScreenManagerClass::ShowHideDeviceRWLMessage(bool showMessage)
{
	if(showMessage)
	{
		UiManager.ChangeVisualTreeRoot(&msg_DeviceRWLState);
	}
	else 
	{ 
		ShowUiMainPage(); 
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
			UiManager.ChangeVisualTreeRoot(&page_Main);
		}
	#endif
}

void ScreenManagerClass::KeyInput(Keys_t key)
{
	UiManager.KeyInput(key);
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