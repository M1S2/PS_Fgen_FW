/*
 * ScreenManager.cpp
 * Created: 05.11.2020 18:45:05
 * Author: Markus Scheich
 */ 

#include <stdio.h>
#include "ScreenManager.h"
#include "../Device.h"

#include "USART/USART.h"

/**
  * Callback method called when the selected tab of the TabControl changed.
  * @param controlContext Not used
  */
void TabControlTabChanged(void* context);

ContainerPageDefault page_Main(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
EnumIndicator<DeviceControlStates_t> enumInd_deviceState(DISPLAY_WIDTH - 70, 2, &Device.DeviceControlState, DeviceControlStateNames, NUM_DEV_CTRL_ELEMENTS);
Label<5> lbl_devSettingsNeedSaving(DISPLAY_WIDTH - 15, 0, "*"); //, u8g_font_7x14r);

TabControlDefault tabControlMain(DISPLAY_WIDTH, DISPLAY_HEIGHT, SCREEN_TAB_WIDTH, TAB_POSITION_LEFT, NULL, &TabControlTabChanged);

MessageDialog<50> msg_DeviceRWLState(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, "Device locked by SYST:RWL.\nUnlock with SYST:LOC.", MSG_WARNING, MSG_BTN_NONE);

ContainerPageDefault page_ScreenPS;
ContainerPageDefault page_ScreenSettings;

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

ScreenManagerClass::ScreenManagerClass() : _tft(LCD_A0_PIN_NUMBER, -1, LCD_CS_PIN_NUMBER) //_tft(LCD_CS_PIN_NUMBER, LCD_A0_PIN_NUMBER)
{
}

void ScreenManagerClass::Init()
{
	_tft.begin();
	_tft.setRotation(1);
	_tft.fillScreen(0x0);

	#ifdef SPLASHSCREEN_ENABLED 
		IsSplashScreenShown = true;
	#endif
	TimeCounter_SplashScreen_ms = 0;
	TimeCounter_ScreenRedraw_ms = 0;
	
	UiManager.Init(&_tft);
	uiBuildTree();
	
	RedrawScreenRequest = true;			// Always draw the screen once when the device is powered on
}

void ScreenManagerClass::UpdateVisibilities()
{
	#ifdef DDS_SUBSYSTEM_ENABLED
		ScreenDDSUpdateVisibility();
	#endif
}

void ScreenManagerClass::uiBuildTree()
{
	#ifdef PS_SUBSYSTEM_ENABLED
		tabControlMain.AddTab("PS", &page_ScreenPS);
		uiBuildScreenPS(&page_ScreenPS);
	#endif
	#ifdef DDS_SUBSYSTEM_ENABLED
		tabControlMain.AddTab("DDS", uiBuildScreenDDS());			// Containing DDS1 and DDS2
	#endif
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		tabControlMain.AddTab("Meas", uiBuildScreenMeasure());		// Containing DMM and ATX measurements
	#endif
	tabControlMain.AddTab("Conf", &page_ScreenSettings);
	uiBuildScreenSettings(&page_ScreenSettings);
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
		UiManager.Draw();
		TimeCounter_ScreenRedraw_ms = 0;
	}
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