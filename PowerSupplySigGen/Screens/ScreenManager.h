/*
 * ScreenManager.h
 *
 * Created: 05.11.2020 18:45:19
 *  Author: V17
 */ 


#ifndef SCREENMANAGER_H_
#define SCREENMANAGER_H_

#include "../libraries/u8glib/u8g.h"
#include "ScreenBase.h"
#include "../KeyPad/KeyPad.h"
#include "../Encoder/Encoder.h"
#include "ScreenPS.h"
#include "ScreenDDS.h"
#include "ScreenDMM.h"
#include "ScreenATX.h"

#include "../Configuration.h"

class ScreenManagerClass
{
	private:
		ScreenPS _screenPs;
		ScreenDDS _screenDds;
		ScreenDMM _screenDmm;
		ScreenATX _screenAtx;
	
		u8g_t _u8g;
		ScreenBase* _screens[NUM_SCREENS];
	    bool _isControlActive;
		bool _displayEnabled;
				
		void drawScreenTabs(int selectedTabIndex);
		void drawMessage();
		void drawPage(bool isFirstPage);
		
		#ifdef SPLASHSCREEN_ENABLED
			void drawSplashScreen();
		#endif
		
	public:
		uint8_t TabIndex;
		bool DisplayInverted;
		char SystemMessage[MAX_MESSAGE_LEN];
		char UserMessage[MAX_MESSAGE_LEN];
	
		bool IsSplashScreenShown;
		uint16_t TimeCounter_SplashScreen_ms;
	
		ScreenManagerClass();
		void Init();
		
		void DrawAll();
		
		void DeviceTimerTickISR(uint16_t currentPeriod_ms);
		
		void KeyInput(Keys_t key);
		void EncoderInput(EncoderDirection_t encDir);
		void EncoderPBInput();
		
		void SetDisplayEnabled(bool displayEnabled);
		bool GetDisplayEnabled();

		void SetDisplayInverted(bool displayInverted);
};

#endif /* SCREENMANAGER_H_ */