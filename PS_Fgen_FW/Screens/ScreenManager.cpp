/*
 * ScreenManager.cpp
 * Created: 05.11.2020 18:45:05
 * Author: Markus Scheich
 */ 

#include <stdio.h>
#include "ScreenManager.h"
#include "../Device.h"
#include "../Spi/spi.h"
#include "../Pins/Pins.h"

#ifdef TOUCH_ENABLED
/*
A: 0.0917605
B: 0.0002037
C: -20.2002980
D: -0.0003954
E: 0.0693526
F: -23.8954410 */
XPT2046::Calibration TS_CALIBRATION = {0.0917605,0.0002037,-20.2002980,-0.0003954,0.0693526,-23.8954410,320,240};
#endif

/**
  * Callback method called when the selected tab of the TabControl changed.
  * @param controlContext Not used
  */
void TabControlTabChanged(void* context);

ContainerPage page_Main(4, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
EnumIndicator<DeviceControlStates_t> enumInd_deviceState(&Device.DeviceControlState, DeviceControlStateNames, NUM_DEV_CTRL_ELEMENTS, DISPLAY_WIDTH - 70, DISPLAY_HEIGHT - STATUS_BAR_HEIGHT + 3);
Label lbl_devSettingsNeedSaving("*", LABEL_COLOR_NOTSET, NULL, DISPLAY_WIDTH - 15, DISPLAY_HEIGHT - STATUS_BAR_HEIGHT + 3, 5);
Icon ico_devCalValid(icon_calibration_width, icon_calibration_height, icon_calibration_bits, ICON_COLOR_NOTSET, DISPLAY_WIDTH - 80 - icon_calibration_width, DISPLAY_HEIGHT - STATUS_BAR_HEIGHT + ((STATUS_BAR_HEIGHT - icon_calibration_height) / 2));

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

ScreenManagerClass::ScreenManagerClass() : _tft(PIN_NUMBER_TFT_CS, PIN_NUMBER_TFT_A0)
#ifdef TOUCH_ENABLED
, _ts(PIN_NUMBER_TOUCH_CS, PIN_NUMBER_TOUCH_IRQ)
#endif
{
}

void ScreenManagerClass::Init()
{
	_tft.begin();
	_tft.setRotation(1);
	_tft.fillScreen(0x0);

#ifdef TOUCH_ENABLED
	_ts.begin();
    _ts.setSampleCount(1);
    _ts.setDebounceTimeout(0);       // !!! It is important to set this to 0 to make the state machine in the TouchHandlingISR() work !!!
    _ts.setCalibration(TS_CALIBRATION);
    _ts.setRotation(_tft.getRotation());
	TimeCounter_TouchHandling_ms = 0;
#endif

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
	page_Main.AddItem(&ico_devCalValid);
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

void ScreenManagerClass::UpdateCalibrationValidIndicator(bool calibrationValid)
{
	ico_devCalValid.Visible = calibrationValid;
	RedrawScreenRequest = true;							// Generate a screen redraw request to update the calibration valid indicator
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
		SPI_SelectDevice(SPI_DEV_TFT);
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

#ifdef TOUCH_ENABLED
	TimeCounter_TouchHandling_ms += DEVICE_TIMER_TICK_INTERVAL_MS;
	if(TimeCounter_TouchHandling_ms >= TOUCH_HANDLING_DELAY_MS)
	{
		TouchHandlingISR();
	}
#endif
}

void ScreenManagerClass::KeyInput(Keys_t key)
{
	UiManager.KeyInput(key);
}

#ifdef TOUCH_ENABLED
bool ScreenManagerClass::TouchInput(uint16_t x, uint16_t y, TouchTypes touchType)
{
	return UiManager.TouchInput(x, y, touchType);
}

void ScreenManagerClass::TouchHandlingISR()
{
	static XPT2046::Point touchedPoint;

    bool isTouched = _ts.touched();
    if(isTouched || _touchEventState != TOUCH_EVENTS_WAIT_FOR_TOUCH)
	{
        switch (_touchEventState)
        {
            case TOUCH_EVENTS_WAIT_FOR_TOUCH:
                if(isTouched)
                {
                    _touchStartTime = millis();
                    touchedPoint = _ts.getTouchPosition();
                    _touchEventState = TOUCH_EVENTS_WAIT_LONG_TOUCH_DELAY;
                }
                break;
            case TOUCH_EVENTS_WAIT_LONG_TOUCH_DELAY:
                if(!isTouched)
                {
                    // Normal touch
                    Device.UserInputHandler.EnqueueTouchInput(touchedPoint.x, touchedPoint.y, TOUCH_NORMAL);
                    _touchEventState = TOUCH_EVENTS_WAIT_FOR_TOUCH;
                }
                else if(millis() - _touchStartTime >= TOUCH_LONG_DELAY_MS)
                {
                    // Long touch
                    Device.UserInputHandler.EnqueueTouchInput(touchedPoint.x, touchedPoint.y, TOUCH_LONG);
                    _touchEventState = TOUCH_EVENTS_LONG_TOUCH_DETECTED;
                }
                break;
            case TOUCH_EVENTS_LONG_TOUCH_DETECTED:
                if(!isTouched)
                {
                    _touchEventState = TOUCH_EVENTS_WAIT_FOR_TOUCH;
                }
                break;
            default: break;
        }
    }
}
#endif