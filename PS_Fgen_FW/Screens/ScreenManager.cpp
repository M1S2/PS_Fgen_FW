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

ContainerPage page_Main(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
EnumIndicator<DeviceControlStates_t> enumInd_deviceState(DISPLAY_WIDTH - 70, DISPLAY_HEIGHT - STATUS_BAR_HEIGHT + 3, &Device.DeviceControlState, DeviceControlStateNames, NUM_DEV_CTRL_ELEMENTS);
Label lbl_devSettingsNeedSaving("*", LABEL_COLOR_NOTSET, NULL, DISPLAY_WIDTH - 15, DISPLAY_HEIGHT - STATUS_BAR_HEIGHT + 3, 5);

#ifdef PS_SUBSYSTEM_ENABLED
	Label labelTabPS("PS", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 5);
	Icon iconTabPS(icon_supplyDC_width, icon_supplyDC_height, icon_supplyDC_bits, COLOR_FOREGROUND_HEADERS);
	ContainerStack stack_TabPSHeader(STACK_LAYOUT_HORIZONTAL_CENTER, 2);
#endif
#ifdef DDS_SUBSYSTEM_ENABLED
	Label labelTabDDS("DDS", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 5);
	Icon iconTabDDS(icon_supplyAC_width, icon_supplyAC_height, icon_supplyAC_bits, COLOR_FOREGROUND_HEADERS);
	ContainerStack stack_TabDDSHeader(STACK_LAYOUT_HORIZONTAL_CENTER, 2);
#endif
#ifdef MEASURE_SUBSYSTEM_ENABLED
	Label labelTabMeas("Meas", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 5);
	Icon iconTabMeas(icon_dmm_width, icon_dmm_height, icon_dmm_bits, COLOR_FOREGROUND_HEADERS);
	ContainerStack stack_TabMeasHeader(STACK_LAYOUT_HORIZONTAL_CENTER, 2);
#endif
Label labelTabConfig("Conf", COLOR_FOREGROUND_HEADERS, NULL, 0, 0, 5);
Icon iconTabConfig(icon_settings_width, icon_settings_height, icon_settings_bits, COLOR_FOREGROUND_HEADERS);
ContainerStack stack_TabConfigHeader(STACK_LAYOUT_HORIZONTAL_CENTER, 2);
TabControl tabControlMain(DISPLAY_WIDTH, DISPLAY_HEIGHT - STATUS_BAR_HEIGHT, TAB_POSITION_TOP, NULL, &TabControlTabChanged);

MessageDialog msg_DeviceRWLState(MSG_DIALOG_MARGIN, MSG_DIALOG_MARGIN, DISPLAY_WIDTH - 2 * MSG_DIALOG_MARGIN, DISPLAY_HEIGHT - 2 * MSG_DIALOG_MARGIN, "Device locked by SYST:RWL.\nUnlock with SYST:LOC.", MSG_WARNING, MSG_BTN_NONE);

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
	
	UiManager.SetColors(COLOR_BACKGROUND, COLOR_FOREGROUND, COLOR_BACKGROUND);
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
		stack_TabPSHeader.AddItem(&iconTabPS);
		stack_TabPSHeader.AddItem(&labelTabPS);
		tabControlMain.AddItem(&stack_TabPSHeader, uiBuildScreenPS());
	#endif
	#ifdef DDS_SUBSYSTEM_ENABLED
		stack_TabDDSHeader.AddItem(&iconTabDDS);
		stack_TabDDSHeader.AddItem(&labelTabDDS);
		tabControlMain.AddItem(&stack_TabDDSHeader, uiBuildScreenDDS());			// Containing DDS1 and DDS2
	#endif
	#ifdef MEASURE_SUBSYSTEM_ENABLED
		stack_TabMeasHeader.AddItem(&iconTabMeas);
		stack_TabMeasHeader.AddItem(&labelTabMeas);
		tabControlMain.AddItem(&stack_TabMeasHeader, uiBuildScreenMeasure());		// Containing DMM and ATX measurements
	#endif
	stack_TabConfigHeader.AddItem(&iconTabConfig);
	stack_TabConfigHeader.AddItem(&labelTabConfig);
	tabControlMain.AddItem(&stack_TabConfigHeader, uiBuildScreenSettings());
	TabControlTabChanged(NULL);				// Trigger a tab changed event to recalculate the CurrentScreenNeedsPeriodicRedraw variable
	
	page_Main.AddItem(&tabControlMain);
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