/*
 * ScreenManager.cpp
 * Created: 05.11.2020 18:45:05
 * Author: Markus Scheich
 */ 

#include <stdio.h>
#include "ScreenManager.h"
#include "../Device.h"

/**
  * Callback method called when the selected tab of the TabControl changed.
  * @param controlContext Not used
  */
void TabControlTabChanged(void* context);

ContainerPage page_Main;
EnumIndicator<DeviceControlStates_t> enumInd_deviceState(240 - 37, 2, &Device.DeviceControlState, DeviceControlStateNames, NUM_DEV_CTRL_ELEMENTS);
Label<5> lbl_devSettingsNeedSaving(240 - 15, 0, "*", u8g_font_7x14r);

TabControl tabControlMain(0, 0, 240, 64, SCREEN_TAB_WIDTH, NULL, &TabControlTabChanged);

MessageDialog<50> msg_DeviceRWLState(0, 0, 240, 64, "Device locked by SYST:RWL.\nUnlock with SYST:LOC.", MSG_WARNING, MSG_BTN_NONE);


void TabControlTabChanged(void* controlContext)
{
	int tabIndex = tabControlMain.GetSelectedTabIndex();
	ScreenTypes_t screenType = (ScreenTypes_t)tabIndex;
	switch(screenType)
	{
	#ifdef PS_SUBSYSTEM_ENABLED
		case SCREEN_PS:
			Device.ScreenManager.CurrentScreenNeedsPeriodicRedraw = true;
			break;
	#endif
	#ifdef DDS_SUBSYSTEM_ENABLED
		case SCREEN_DDS:
			Device.ScreenManager.CurrentScreenNeedsPeriodicRedraw = false;
			break;
	#endif
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		case SCREEN_MEAS:
			Device.ScreenManager.CurrentScreenNeedsPeriodicRedraw = true;
			break;
	#endif
		case SCREEN_CONF:
			Device.ScreenManager.CurrentScreenNeedsPeriodicRedraw = false;
			break;
		default: break;		
	}
}


void ScreenManagerClass::Init()
{
	u8g_InitSPI(&_u8g, &u8g_dev_s1d15721_hw_spi, PN(1, 7), PN(1, 5), PN(1, 1), PN(1, 0), U8G_PIN_NONE);
	IsSplashScreenShown = true;
	TimeCounter_SplashScreen_ms = 0;
	TimeCounter_ScreenRedraw_ms = 0;
	
	UiManager.Init(&_u8g);
	uiBuildTree();
	
	RedrawScreenRequest = true;			// Always draw the screen once when the device is powered on
}

void ScreenManagerClass::UpdateVisibilities()
{
	ScreenDDSUpdateVisibility();
}

void ScreenManagerClass::uiBuildTree()
{
	#ifdef PS_SUBSYSTEM_ENABLED
		tabControlMain.AddTab("PS", uiBuildScreenPS());	
	#endif
	#ifdef DDS_SUBSYSTEM_ENABLED
		tabControlMain.AddTab("DDS", uiBuildScreenDDS());			// Containing DDS1 and DDS2
	#endif
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		tabControlMain.AddTab("Meas", uiBuildScreenMeasure());		// Containing DMM and ATX measurements
	#endif
	tabControlMain.AddTab("Conf", uiBuildScreenSettings());
	TabControlTabChanged(NULL);				// Trigger a tab changed event to recalculate the CurrentScreenNeedsPeriodicRedraw variable
	
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
	RedrawScreenRequest = true;							// Generate a screen redraw request to update the settings saved indicator
}

void ScreenManagerClass::DoDraw()
{
	if(TimeCounter_ScreenRedraw_ms >= SCREEN_REDRAW_DELAY_MS)		// Create a redraw request if the redraw delay is expired
	{
		RedrawScreenRequest = true;
		
		#ifdef MEASURE_SUBSYSTEM_ENABLED
			updateDmmNegativeWarningVisibility();
		#endif
	}
	
	if(RedrawScreenRequest)
	{
		RedrawScreenRequest = false;			// Reset redraw request to only draw once (until the request is set to true again)
		DrawAll();
		TimeCounter_ScreenRedraw_ms = 0;
	}
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

void ScreenManagerClass::DeviceTimerTickISR()
{
	#ifdef SPLASHSCREEN_ENABLED
		if(IsSplashScreenShown) { TimeCounter_SplashScreen_ms += DEVICE_TIMER_TICK_INTERVAL_MS; }
			
		/* Hide splash screen after some time */
		if(IsSplashScreenShown && (TimeCounter_SplashScreen_ms >= SPLASHSCREEN_DELAY_MS))
		{
			IsSplashScreenShown = false;
			UiManager.ChangeVisualTreeRoot(&page_Main);
			RedrawScreenRequest = true;							// Generate a screen redraw request to update the screen
		}
	#endif
	
	if(CurrentScreenNeedsPeriodicRedraw)		// Only increment the redraw counter, if the current screen needs periodic redraws. Otherwise the redraw is triggered by the RedrawScreenRequest variable
	{
		TimeCounter_ScreenRedraw_ms += DEVICE_TIMER_TICK_INTERVAL_MS;	// Screen redraw is handled in DoDraw()
	}
}

void ScreenManagerClass::KeyInput(Keys_t key)
{
	UiManager.KeyInput(key);
}