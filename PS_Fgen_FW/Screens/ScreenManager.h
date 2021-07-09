/*
 * ScreenManager.h
 * Created: 05.11.2020 18:45:19
 * Author: Markus Scheich
 */ 

#ifndef SCREENMANAGER_H_
#define SCREENMANAGER_H_

#include "../libraries/u8glib/u8g.h"
#include "../Encoder/Encoder.h"
#include "../KeyPad/KeyPad.h"
#include "../Configuration.h"
#include "../UI_Lib/Core/UI_Manager.h"
#include "../UI_Lib/Core/UI_Elements.h"
#include "Icons.h"
#include <string.h>

#ifdef SPLASHSCREEN_ENABLED 
	UIElement* uiBuildSplashScreen();
#endif
UIElement* uiBuildScreenPS();
UIElement* uiBuildScreenDDS();
UIElement* uiBuildScreenMeasure();
UIElement* uiBuildScreenSettings();

UIElement* uiBuildScreenCalibration();
UIElement* StartCalibration();

class ScreenManagerClass
{
	private:
		void uiBuildTree();
	
		u8g_t _u8g;
	    bool _isControlActive;
		bool _displayEnabled;
		
		#ifdef SPLASHSCREEN_ENABLED
			void drawSplashScreen();
		#endif
		
	public:
		UI_Manager UiManager;
		void ShowUiMainPage();
		void ShowUiCalibrationMenu();
		
		bool DisplayInverted;
	
		bool IsSplashScreenShown;
		uint16_t TimeCounter_SplashScreen_ms;
	
		ScreenManagerClass();
		void Init();
		
		void DrawAll();
		
		void ShowHideDeviceRWLMessage(bool showMessage);
		
		void DeviceTimerTickISR(uint16_t currentPeriod_ms);
		
		void UpdateSettingsChangedIndicator(bool settingsChanged);
		
		void KeyInput(Keys_t key);
		
		void SetDisplayEnabled(bool displayEnabled);
		bool GetDisplayEnabled();

		void SetDisplayInverted(bool displayInverted);
};

#endif /* SCREENMANAGER_H_ */