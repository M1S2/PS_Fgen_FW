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

ContainerPage page_Main(3, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
EnumIndicator<DeviceControlStates_t> enumInd_deviceState(&Device.DeviceControlState, DeviceControlStateNames, NUM_DEV_CTRL_ELEMENTS, DISPLAY_WIDTH - 70, DISPLAY_HEIGHT - STATUS_BAR_HEIGHT + 3);
Label lbl_devSettingsNeedSaving("*", LABEL_COLOR_NOTSET, NULL, DISPLAY_WIDTH - 15, DISPLAY_HEIGHT - STATUS_BAR_HEIGHT + 3, 5);

#ifdef PS_SUBSYSTEM_ENABLED
	Icon iconTabPS(icon_supplyDC_32x32_width, icon_supplyDC_32x32_height, icon_supplyDC_32x32_bits, COLOR_FOREGROUND_HEADERS);
#endif
#ifdef DDS_SUBSYSTEM_ENABLED
	Icon iconTabDDS(icon_supplyAC_32x32_width, icon_supplyAC_32x32_height, icon_supplyAC_32x32_bits, COLOR_FOREGROUND_HEADERS);
#endif
#ifdef MEASURE_SUBSYSTEM_ENABLED
	Icon iconTabMeas(icon_dmm_32x32_width, icon_dmm_32x32_height, icon_dmm_32x32_bits, COLOR_FOREGROUND_HEADERS);
#endif
Icon iconTabConfig(icon_settings_32x32_width, icon_settings_32x32_height, icon_settings_32x32_bits, COLOR_FOREGROUND_HEADERS);
ContainerTabs containerTabsMain(DISPLAY_WIDTH, DISPLAY_HEIGHT - STATUS_BAR_HEIGHT, TAB_POSITION_LEFT, NULL, &TabControlTabChanged);

MessageDialog msg_DeviceRWLState(MSG_DIALOG_MARGIN, MSG_DIALOG_MARGIN, DISPLAY_WIDTH - 2 * MSG_DIALOG_MARGIN, DISPLAY_HEIGHT - 2 * MSG_DIALOG_MARGIN, "Device locked by SYST:RWL.\nUnlock with SYST:LOC.", MSG_WARNING, MSG_BTN_NONE);

void TabControlTabChanged(void* controlContext)
{
	int tabIndex = containerTabsMain.GetSelectedTabIndex();
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

ScreenManagerClass::ScreenManagerClass() : _tft(LCD_A0_PIN_NUMBER, -1, LCD_CS_PIN_NUMBER)
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
	
	UiManager.SetColors(COLOR_BACKGROUND, COLOR_FOREGROUND, COLOR_BACKGROUND, COLOR_FOCUS_FRAME);
	UiManager.Init(&_tft);
	UiManager.ElementMargin = 2;
	UiManager.ElementPadding = 1;
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
		containerTabsMain.AddItem(&iconTabPS, uiBuildScreenPS());
	#endif
	#ifdef DDS_SUBSYSTEM_ENABLED
		containerTabsMain.AddItem(&iconTabDDS, uiBuildScreenDDS());			// Containing DDS1 and DDS2
	#endif
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		containerTabsMain.AddItem(&iconTabMeas, uiBuildScreenMeasure());		// Containing DMM and ATX measurements
	#endif
	containerTabsMain.AddItem(&iconTabConfig, uiBuildScreenSettings());
	TabControlTabChanged(NULL);				// Trigger a tab changed event to recalculate the CurrentScreenNeedsPeriodicRedraw variable
	
	page_Main.AddItem(&containerTabsMain);
	page_Main.AddItem(&enumInd_deviceState);
	page_Main.AddItem(&lbl_devSettingsNeedSaving);
	page_Main.InitItems();
	page_Main.RecalculateLayout();
	
	uiBuildScreenCalibration();
	
	#ifdef SPLASHSCREEN_ENABLED 
		UiManager.ChangeVisualTreeRoot(uiBuildSplashScreen());
	#else
		UiManager.ChangeVisualTreeRoot(&page_Main);
	#endif
}

void ScreenManagerClass::ShowUiMainPage()
{
	UiManager.Gfx->setTextWrap(false);
	UiManager.ChangeVisualTreeRoot(&page_Main);
}

void ScreenManagerClass::ShowUiCalibrationMenu()
{
	UiManager.Gfx->setTextWrap(true);
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